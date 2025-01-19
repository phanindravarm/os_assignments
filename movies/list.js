import { watchListMovies, removeFromWatchList } from "./watchListMovies.js";
export function renderWatchList() {
  let movieHtml = "";
  watchListMovies.forEach((movie) => {
    movieHtml += `<div class="js-${movie.Title}">
      <h3>${movie.Title}</h3>
      <button class="delete-button" data-movie-title="${movie.Title}">delete</button>
    </div>`;
  });
  document.querySelector(".watch-list").innerHTML = movieHtml;
}
function saveToStorage() {
  localStorage.setItem("watchListMovies", JSON.stringify(watchListMovies));
}
document.querySelector(".watch-list").addEventListener("click", (event) => {
  if (event.target.classList.contains("delete-button")) {
    const movieTitle = event.target.dataset.movieTitle;
    removeFromWatchList(movieTitle);
    saveToStorage();
    renderWatchList();
  }
});
saveToStorage();
renderWatchList();
