# Form template

```js
import './index.css';

const Form = ({ onSubmit }) => {
  const [formData, setFormData] = React.useState({
    firstName: '',
    lastName: '',
    email: '',
    comments: '',
    isFriendly: true,
    employment: 'full-time',
    favoriteColor: ''
  });

  const handleChange = (event) => {
    const { name, value, type, checked } = event.target;
    setFormData(formData => ({
      ...formData,
      [name]: type === 'checkbox' ? checked : value
    }));
  };

  const handleSubmit = (event) => {
    event.preventDefault();
    onSubmit(formData);   // pass up
  };

  return (
    <form className='form' onSubmit={handleSubmit}>
      <input onChange={handleChange} type='text'
        name='firstName' placeholder='First Name' value={formData.firstName} />
      <input onChange={handleChange} type='text'
        name='lastName' placeholder='Last Name' value={formData.lastName} />
      <input onChange={handleChange} type='text'
        name='email' placeholder='Email' value={formData.email} />
      <input onChange={handleChange} type='textarea'
        name='comments' placeholder='Comments' value={formData.comments} />
        
      <div>
        <input onChange={handleChange} type='checkbox'
          name='isFriendly' id='isFriendly' checked={formData.isFriendly} />
        <label htmlFor='isFriendly'>Are you friendly?</label>
      </div>

      <fieldset>
        <legend>Current emplyment status</legend>
        <div>
          <input onChange={handleChange} type='radio' 
            name='employment' id='unemployed' value='unemployed'
            checked={formData.employment === 'unemployed'} />
          <label htmlFor='unemployed'>Unemployed</label>
        </div>

        <div>
          <input onChange={handleChange} type='radio' 
            name='employment' id='part-time' value='part-time'
            checked={formData.employment === 'part-time'} />
          <label htmlFor='part-time'>Part-time</label>
        </div>

        <div>
          <input onChange={handleChange} type='radio' 
            name='employment' id='full-time' value='full-time'
            checked={formData.employment === 'full-time'} />
          <label htmlFor='full-time'>Full-time</label>
        </div>
      </fieldset>

      <div>
          <label htmlFor='favoriteColor'>What is your favorite color?</label>
          <select onChange={handleChange} name='favoriteColor' id='favoriteColor' value={formData.favoriteColor}>
            <option value=''>-- Choose --</option>
            <option value='red'>Red</option>
            <option value='green'>Green</option>
            <option value='blue'>Blue</option>
          </select>
        </div>

        <button className='btn submit'>Submit</button>
    </form>
  );
};
```
