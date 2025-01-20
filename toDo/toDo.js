import { v4 as uuidv4 } from "https://cdn.jsdelivr.net/npm/uuid@8.3.2/dist/esm-browser/index.js";
function enterHandler(event) {
  if (event.key === "Enter") {
    addTodo();
  }
}

function addTodo() {
  let element = document.querySelector(".todo-input");
  let task = element.value.trim();
  if (task === "") {
    return;
  }
  const newTask = {
    task: task,
    id: uuidv4(),
  };
  todoList.push(newTask);
  element.value = "";
  saveToLocalStorage();
  renderList();
}

function renderList() {
  let listHtml = "";
  todoList.forEach((toDo) => {
    listHtml += `<li class="todo-list-element" data-id="${toDo.id}">${toDo.task}
        <button class="todo-delete-button" data-id="${toDo.id}">Delete</button>
        <button class="todo-done-button" data-id="${toDo.id}">Done</button>
      </li>`;
  });
  document.querySelector(".todo-list").innerHTML = listHtml;

  document.querySelectorAll(".todo-delete-button").forEach((button) => {
    button.addEventListener("click", (event) => {
      const id = event.target.dataset.id;
      deleteTodo(id);
    });
  });

  document.querySelectorAll(".todo-done-button").forEach((button) => {
    button.addEventListener("click", (event) => {
      const id = event.target.dataset.id;
      addDone(id);
    });
  });
}

function renderDoneList() {
  let listHtml = "";
  doneList.forEach((toDo) => {
    listHtml += `<li class="done-list-element" data-id="${toDo.id}">${toDo.task}
                        <button class="done-delete-button" data-id="${toDo.id}">Delete</button>
                  </li>`;
  });
  document.querySelector(".done-list").innerHTML = listHtml;

  document.querySelectorAll(".done-delete-button").forEach((button) => {
    button.addEventListener("click", (event) => {
      const id = event.target.dataset.id;
      deleteDone(id);
    });
  });
}

function saveToLocalStorage() {
  localStorage.setItem("todoList", JSON.stringify(todoList));
  localStorage.setItem("doneList", JSON.stringify(doneList));
}

function loadFromLocalStorage() {
  const savedTodoList = localStorage.getItem("todoList");
  const savedDoneList = localStorage.getItem("doneList");
  if (savedTodoList) {
    todoList = JSON.parse(savedTodoList);
  }
  if (savedDoneList) {
    doneList = JSON.parse(savedDoneList);
  }
}

function clearLocalStorage(listName, listArray) {
  localStorage.removeItem(listName);
  listArray.length = 0;
  renderList();
  renderDoneList();
}

function deleteTodo(id) {
  todoList = todoList.filter((list) => list.id !== id);
  saveToLocalStorage();
  renderList();
}

function addDone(id) {
  const doneTask = todoList.find((list) => list.id === id);
  if (doneTask) {
    todoList = todoList.filter((list) => list.id !== id);
    doneList.push(doneTask);
    saveToLocalStorage();
    renderList();
    renderDoneList();
  }
}

function deleteDone(id) {
  doneList = doneList.filter((list) => list.id !== id);
  saveToLocalStorage();
  renderDoneList();
}

let todoList = [];
let doneList = [];
loadFromLocalStorage();

document.querySelector(".todo-button").addEventListener("click", () => {
  addTodo();
});
document.querySelector(".todo-input").addEventListener("keydown", (event) => {
  enterHandler(event);
});
document.querySelector(".do-reset-button").addEventListener("click", () => {
  clearLocalStorage("todoList", todoList);
});
document.querySelector(".done-reset-button").addEventListener("click", () => {
  clearLocalStorage("doneList", doneList);
});

renderList();
renderDoneList();
