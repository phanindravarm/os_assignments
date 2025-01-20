let score = JSON.parse(localStorage.getItem("score")) || {
  win: 0,
  lose: 0,
  tie: 0,
};

function play(playerMove) {
  let computerMove = computerPick();
  let result = "";
  if (playerMove === "scissors") {
    if (computerMove === "rock") {
      result = "you lose";
    } else if (computerMove === "paper") {
      result = "you win";
    } else if (computerMove === "scissors") {
      result = "tie";
    }
  } else if (playerMove === "paper") {
    if (computerMove === "rock") {
      result = "you win";
    } else if (computerMove === "paper") {
      result = "tie";
    } else if (computerMove === "scissors") {
      result = "you lose";
    }
  } else if (playerMove === "rock") {
    if (computerMove === "rock") {
      result = "tie";
    } else if (computerMove === "paper") {
      result = "you lose";
    } else if (computerMove === "scissors") {
      result = "you win";
    }
  }
  if (result === "you win") {
    score.win = score.win + 1;
  } else if (result === "you lose") {
    score.lose = score.lose + 1;
  } else if (result === "tie") {
    score.tie = score.tie + 1;
  }
  localStorage.setItem("score", JSON.stringify(score));
  document.querySelector(".result").innerHTML = result;
  document.querySelector(
    ".score"
  ).innerHTML = `Wins: ${score.win}, Losses: ${score.lose}, Ties: ${score.tie}`;
}
function resetGame() {
  localStorage.removeItem("score");
  score = { win: 0, lose: 0, tie: 0 };
  document.querySelector(
    ".score"
  ).innerHTML = `Wins: ${score.win}, Losses: ${score.lose}, Ties: ${score.tie}`;
  document.querySelector(".result").innerHTML = "";
}
function computerPick() {
  let move = Math.random();
  if (move < 1 / 3) {
    return "rock";
  } else if (move < 2 / 3) {
    return "paper";
  } else return "scissors";
}

play();
