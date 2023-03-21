import {useState} from 'react';

const List = () => {
    const [elements, getElements] = useState(JSON.parse(localStorage.getItem("todolist"))||[]);
  
  const setAndSaveItems = (newItems) => {
    getElements(newItems);
    localStorage.setItem('todolist', JSON.stringify(newItems));
  }

  const handleDelete = (id)=>{
      const listItem = elements.filter(item=>item.id!=id);
      setAndSaveItems(listItem);
  }
  
  const [newItem, setNewItem] = useState('');
  
  const handleSubmit = (e) =>{
    e.preventDefault();
    addItem(newItem);
    setNewItem("");
  }
  
  const addItem = (item) =>{
    const id = elements.length ? elements[elements.length-1].id+1 : 1;
    const myNewItem = {
      id,
      checked: false,
      item
    }
    const listItem = [...elements, myNewItem];
    setAndSaveItems(listItem);
  }

  return (
    <div class = "container">
        <form onSubmit = {handleSubmit}>
            <input type = "text" required placeholder = "Add Item" value = {newItem} onChange = {(e)=>setNewItem(e.target.value)}/>
            <button type = "submit">
                Add
            </button>
        </form>
        {elements.length? (
            <ul>
                {elements.map(item=>(
                    <li>
                        <p>{item.item}</p>
                        <button class = "b" onClick = {()=>handleDelete(item.id)}>
                            Completed
                        </button>
                    </li>
                ))}
            </ul>) : 
            (
                <h2>You Completed Your To-Do List</h2>
            )
        }
    </div>
  )
}

export default List