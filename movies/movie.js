import { moviesCart } from "./moviesCart.js";
import { addToWatchList } from "./watchListMovies.js";

function renderMovieList() {
  let dataHtml = "";
  let moviesList = document.querySelector(".ul-movies");
  moviesCart.forEach((movie) => {
    dataHtml += `<div class="movie-item">
        <h3>${movie.Title}</h3>
        <img class="movie-image" src="${movie.Poster}">
        <button class="add-to-watchList" data-movie-title="${movie.Title}">Add</button>
        </div>`;
  });

  moviesList.innerHTML = dataHtml;
}
renderMovieList();
document.querySelectorAll(".add-to-watchList").forEach((button) => {
  button.addEventListener("click", () => {
    const movieName = button.dataset.movieTitle;
    addToWatchList(movieName);
  });
});
