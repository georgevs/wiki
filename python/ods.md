# ODS files

### Install dependencies
```bash
pip3 install \
  ezodf \
  lxml
```

### ODS utils
```python
from ezodf.bytestreammanager import ByteStreamManager
from ezodf.document import PackagedDocument
import ezodf
import itertools


class ODS:
  def load_doc(*, data=None, file_path=None):
    if data:  
      mimetype = 'application/vnd.oasis.opendocument.spreadsheet'
      filemanager = ByteStreamManager(data)
      return ODS(doc=PackagedDocument(filemanager=filemanager, mimetype=mimetype))
    elif file_path:
      return ODS(doc=ezodf.opendoc(file_path))
  
  def new_doc(file_path=None):
    return ODS(doc=ezodf.newdoc(doctype='ods', filename=file_path or ''))

  def __init__(self, doc):
    assert doc.doctype == 'ods'
    self.doc = doc
  
  def save_doc(self, file_path=None, backup=False):
    self.doc.backup = backup
    if file_path:
      self.doc.saveas(file_path)
    else:
      self.doc.save()

  def to_bytes_doc(self):
    return self.doc.tobytes()

  def load_sheet(self, sheet_name, sheet_columns):
    table = self.doc.sheets[sheet_name]
    rows = table.rows()
    header_row = next(rows)
    columns_selectors = [cell.value in sheet_columns for cell in header_row]
    row_values = lambda row: [cell.value for cell in itertools.compress(row, columns_selectors)]
    cols = row_values(header_row)
    rows = [row_values(row) for row in rows]
    return cols, rows

  def sheet_for(sheet_name, cols, rows):
    rows = list(rows)
    nrows, ncols = len(rows), len(cols)
    sheet = ezodf.Sheet(name=sheet_name, size=(nrows+1, ncols))
    for i in range(ncols):
      sheet[0,i].set_value(cols[i])
    for j in range(nrows):
      for i in range(ncols):
        if (val := rows[j][i]) != None:
          sheet[j+1,i].set_value(val)
    return sheet

  def update_sheet(self, sheet, index=0):
    if sheet.name in self.doc.sheets.names():
      self.doc.sheets[sheet.name] = sheet
    else:
      self.doc.sheets.insert(index, sheet)
```

### Load/Save sheet
```python
class Bal(DS):
  def load_sheet(ods, sheet_name='BAL'):
    sheet_columns = ['DT', 'AMNT', 'GCASH', 'U3947', 'U9271', 'VCASH', 'VLOAN', 'NOTE']
    cols, rows = ods.load_sheet(sheet_name, sheet_columns)
    return Bal(sheet_name, cols, rows)
  
  def __init__(self, sheet_name, cols, rows):
    super().__init__(cols, rows)
    self.sheet_name = sheet_name
    self.note = self.col('NOTE')

  def update_sheet(self, ods, *, sheet_name=None, index=0):
    sheet = ODS.sheet_for(sheet_name or self.sheet_name, self.cols(), self.rows())
    ods.update_sheet(sheet=sheet, index=index)


def groom_spreadsheet(self, file_path=None):
  ods = ODS.load_doc(file_path=file_path)

  bal = Bal.load_sheet(ods)
  bal = bal.drop_empty()
  bal = bal.add_acct()

  ods = ODS.new_doc()
  bal.update_sheet(ods)

  file_name, ext = os.path.splitext(file_path)
  groomed_file_path = f'{file_name}-groomed{ext}'
  ods.save_doc(file_path=groomed_file_path)
```
