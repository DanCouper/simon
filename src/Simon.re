/* ---------------------------
 * Helpers
 * --------------------------- */
/* `ReasonReact.stringToElement` is a little verbose, so alias it.
 * NOTE this particular vagary get annoying quickly. */
let stringToEl = ReasonReact.stringToElement;

/* ---------------------------
 * SequenceDisplay component
 *
 * The core component, shifts between game states.
 * --------------------------- */
type gameState =
  | Waiting
  | Ready
  | Displaying
  | Displayed
  | Playing;

type action =
  | Start
  | Display
  | DisplayComplete
  | Play
  | RoundComplete
  | IncorrectGuess;

let nextRound (round: int) (maxRounds: int) =>
  switch round {
  | x when x > maxRounds => 0
  | _ => round + 1
  };

let growSeq seq round keys maxRounds =>
  switch round {
  | x when x > maxRounds => []
  | _ => seq @ [Random.int keys]
  };

type state = {
  gameState,
  round: int,
  seq: list int,
  restarts: int,
  transitionMessage: string
};

let component = ReasonReact.reducerComponent "Simon";

let make ::maxRounds=20 ::numKeys=4 ::maxRetries=3 _children => {
  ...component,
  initialState: fun () => {
    gameState: Waiting,
    restarts: maxRetries,
    round: 0,
    seq: [],
    transitionMessage: "Play!"
  },
  reducer: fun action state =>
    switch action {
    | Start =>
      let round = nextRound state.round maxRounds;
      ReasonReact.Update {
        ...state,
        gameState: Ready,
        round,
        seq: growSeq state.seq state.round numKeys maxRounds,
        transitionMessage: "Time to go! Start round " ^ string_of_int round
      }
    | Display => ReasonReact.Update {...state, gameState: Displaying}
    | DisplayComplete =>
      ReasonReact.Update {
        ...state,
        gameState: Displayed,
        transitionMessage: "Now you've seen the sequence, can you get it right?"
      }
    | Play => ReasonReact.Update {...state, gameState: Playing}
    | RoundComplete =>
      ReasonReact.Update {
        ...state,
        gameState: Waiting,
        transitionMessage:
          "Round " ^
          string_of_int state.round ^ " complete! Ready to play again?"
      }
    | IncorrectGuess =>
      switch state.restarts {
      | 0 =>
        ReasonReact.Update {
          gameState: Waiting,
          transitionMessage: "Game Over! You're right back at the start :(",
          round: 0,
          seq: [],
          restarts: maxRetries
        }
      | x =>
        ReasonReact.Update {
          ...state,
          gameState: Ready,
          restarts: x - 1,
          transitionMessage:
            "Oops! Only " ^
            string_of_int (x - 1) ^ " tries left! Lets watch the sequence again!"
        }
      }
    },
  render: fun self =>
    <figure>
      (
        switch self.state.gameState {
        | Waiting =>
          <button onClick=(self.reduce (fun _ => Start))>
            (stringToEl self.state.transitionMessage)
          </button>
        | Ready =>
          <button onClick=(self.reduce (fun _ => Display))>
            (stringToEl self.state.transitionMessage)
          </button>
        | Displaying =>
          <SequenceDisplay
            displaySeq=self.state.seq
            timeoutDelay=500
            displayEndNotifier=(self.reduce (fun _ => DisplayComplete))
          />
        | Displayed =>
          <button onClick=(self.reduce (fun _ => Play))>
            (stringToEl self.state.transitionMessage)
          </button>
        | Playing =>
          <SequenceInput
            keys=numKeys
            checkSeq=self.state.seq
            roundCompleteNotifier=(self.reduce (fun _ => RoundComplete))
            gameOverNotifier=(self.reduce (fun _ => IncorrectGuess))
          />
        }
      )
    </figure>
};
