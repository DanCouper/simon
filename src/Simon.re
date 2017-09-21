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

let gameStateToReactStr gameState =>
  switch gameState {
  | Waiting => stringToEl "Game state: Waiting"
  | Ready => stringToEl "Game state: Ready"
  | Displaying => stringToEl "Game state: Displaying"
  | Displayed => stringToEl "Game state: Displayed"
  | Playing => stringToEl "Game state: Playing"
  };

type action =
  | Start
  | Display
  | DisplayComplete
  | Play;

type state = {gameState, seq: list int};

let component = ReasonReact.reducerComponent "Simon";

let make ::rounds ::keys _children => {
  ...component,
  initialState: fun () => {gameState: Waiting, seq: []},
  reducer: fun action state =>
    switch action {
    | Start => ReasonReact.Update {...state, gameState: Ready}
    | Display => ReasonReact.Update {...state, gameState: Displaying}
    | DisplayComplete => ReasonReact.Update {...state, gameState: Displayed}
    | Play => ReasonReact.Update {...state, gameState: Playing}
    },
  render: fun self =>
    <figure>
      <p> (gameStateToReactStr self.state.gameState) </p>
      (
        switch self.state.gameState {
        | Displaying =>
          <SequenceDisplay
            displaySeq=[1, 2, 3, 4, 5, 6, 7]
            timeoutDelay=500
            displayEndNotifier=(self.reduce (fun _ => DisplayComplete))
          />
        | Playing => <SequenceInput inputs=[{keyValue: 0}, {keyValue: 1}, {keyValue: 2}, {keyValue: 3}] />
        | _ => ReasonReact.nullElement
        }
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
        | Playing => ReasonReact.nullElement
        }
      )
    </figure>
};
