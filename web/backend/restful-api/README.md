# RESTful API 

## References

[HTTP methods](https://www.restapitutorial.com/lessons/httpmethods.html)
[RESTful web API design](https://learn.microsoft.com/en-us/azure/architecture/best-practices/api-design)

## Create
```
POST /customers
  201 (Created), 'Location' header with link to /customers/{id} containing new ID.
POST /customers/{id}
  404 (Not Found), 409 (Conflict) if resource already exists.
```
## Read
```
GET /customers
  200 (OK), list of customers. Use pagination, sorting and filtering to navigate big lists.
GET /customers/{id}
  200 (OK), single customer. 404 (Not Found), if ID not found or invalid.
```
## Update/Replace
```
PUT /customers
  405 (Method Not Allowed), unless you want to update/replace every resource in the entire collection.
PUT /customers/{id}
  200 (OK) or 204 (No Content). 404 (Not Found), if ID not found or invalid.
```
## Update/Modify
```
PATCH /customers
  405 (Method Not Allowed), unless you want to modify the collection itself.
PATCH /customers/{id}
  200 (OK) or 204 (No Content). 404 (Not Found), if ID not found or invalid.
```
## Delete
```
DELETE /customers
  405 (Method Not Allowed), unless you want to delete the whole collection—not often desirable.
DELETE /customers/{id}
  200 (OK). 404 (Not Found), if ID not found or invalid.
```
