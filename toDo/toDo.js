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
  todoList.push(task);
  element.value = "";
  saveToLocalStorage();
  renderList();
}

function renderList() {
  let listHtml = "";
  todoList.forEach((toDo) => {
    listHtml += `<li class="todo-list-element">${toDo}
                       <button class="todo-delete-button">Delete</button>
                       <button class="todo-done-button">Done</button>
                     </li>`;
  });
  document.querySelector(".todo-list").innerHTML = listHtml;

  document
    .querySelectorAll(".todo-delete-button")
    .forEach((deleteButton, index) => {
      deleteButton.addEventListener("click", () => {
        todoList.splice(index, 1);
        saveToLocalStorage();
        renderList();
      });
    });

  document
    .querySelectorAll(".todo-done-button")
    .forEach((doneButton, index) => {
      doneButton.addEventListener("click", () => {
        let doneTask = todoList.splice(index, 1)[0];
        doneList.push(doneTask);
        saveToLocalStorage();
        renderList();
        renderDoneList();
      });
    });
}

function renderDoneList() {
  let listHtml = "";
  doneList.forEach((toDo) => {
    listHtml += `<li class="done-list-element">${toDo}
                       <button class="done-delete-button">Delete</button>
                     </li>`;
  });
  document.querySelector(".done-list").innerHTML = listHtml;

  document
    .querySelectorAll(".done-delete-button")
    .forEach((deleteButton, index) => {
      deleteButton.addEventListener("click", () => {
        doneList.splice(index, 1);
        saveToLocalStorage();
        renderDoneList();
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
