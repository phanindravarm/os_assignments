export let watchListMovies = JSON.parse(
  localStorage.getItem("watchListMovies")
);

if (!watchListMovies) {
  watchListMovies = [
    {
      Title: "The Shawshank Redemption",
    },
  ];
}

function saveToStorage() {
  localStorage.setItem("watchListMovies", JSON.stringify(watchListMovies));
}

export function addToWatchList(movieName) {
  let matchingMovie;
  watchListMovies.forEach((movie) => {
    if (movie.Title === movieName) {
      matchingMovie = movie;
    }
  });
  if (!matchingMovie) {
    watchListMovies.push({
      Title: movieName,
    });
  }
  console.log("adding");
  console.log(watchListMovies);

  saveToStorage();
}

export function removeFromWatchList(movieTitle) {
  const index = watchListMovies.findIndex(
    (movie) => movie.Title === movieTitle
  );

  if (index !== -1) {
    console.log("removing");
    console.log(watchListMovies);
    watchListMovies.splice(index, 1);
    saveToStorage();
  }
}
