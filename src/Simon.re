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
  | Displayed;

let gameStateToReactStr gameState =>
  switch gameState {
  | Waiting => stringToEl "Game state: Waiting"
  | Ready => stringToEl "Game state: Ready"
  | Displaying => stringToEl "Game state: Displaying"
  | Displayed => stringToEl "Game state: Displayed"
  };

type action =
  | Start
  | Display
  | Play;

type state = {gameState, seq: list int};

let component = ReasonReact.reducerComponent "Simon";

let make _children => {
  ...component,
  initialState: fun () => {gameState: Waiting, seq: []},
  reducer: fun action state =>
    switch action {
    | Start => ReasonReact.Update {...state, gameState: Ready}
    | Display => ReasonReact.Update {...state, gameState: Displaying}
    | Play => ReasonReact.NoUpdate
    },
  render: fun self =>
    <figure>
      <p> (gameStateToReactStr self.state.gameState) </p>
      (
        self.state.gameState == Displaying ?
          <SequenceDisplay
            displaySeq=[1, 2, 3, 4, 5, 6, 7]
            timeoutDelay=500
          /> :
          ReasonReact.nullElement
      )
      (
        switch self.state.gameState {
        | Waiting =>
          <button onClick=(self.reduce (fun _ => Start))>
            (stringToEl "Start Round")
          </button>
        | Ready =>
          <button onClick=(self.reduce (fun _ => Display))>
            (stringToEl "Display Sequence")
          </button>
        | Displaying => ReasonReact.nullElement
        | Displayed =>
          <button onClick=(self.reduce (fun _ => Play))>
            (stringToEl "Start Guessing")
          </button>
        }
      )
    </figure>
};
