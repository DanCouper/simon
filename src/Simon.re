/* ---------------------------
 * Helpers
 * --------------------------- */
/* `ReasonReact.stringToElement` is a little verbose, so alias it.
 * NOTE this particular vagary get annoying quickly. */
let stringToEl = ReasonReact.stringToElement;

/* ReasonReact doesn't play nice with React Devtools. This is a pain
 * point; for development, manually printing out the state turns out
 * to be helpful: */
let gameSeqToReactString seq =>
  stringToEl ("Current sequence: " ^ Helpers.intListToString seq);

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
  | Play
  | Check int;

type inputModel = {key: int, callback: ReactEventRe.Mouse.t => unit};

let buildInputModelArray numKeys self =>
  Helpers.buildRange numKeys |>
  List.map (fun k => {key: k, callback: self.reduce (fun _ => Check k)}) |> Array.of_list;

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

type state = {gameState, round: int, seq: list int, checkSeq: list int};

let component = ReasonReact.reducerComponent "Simon";

let make ::maxRounds=20 ::numKeys=4 _children => {
  ...component,
  initialState: fun () => {gameState: Waiting, round: 0, seq: [], checkSeq: []},
  reducer: fun action state =>
    switch action {
    | Start =>
      ReasonReact.Update {
        ...state,
        gameState: Ready,
        round: nextRound state.round maxRounds,
        seq: growSeq state.seq state.round numKeys maxRounds
      }
    | Display => ReasonReact.Update {...state, gameState: Displaying}
    | DisplayComplete => ReasonReact.Update {...state, gameState: Displayed}
    | Play => ReasonReact.Update {...state, gameState: Playing}
    | Check _ => ReasonReact.NoUpdate
    },
  render: fun self =>
    <figure>
      <header>
        <p> (gameStateToReactStr self.state.gameState) </p>
        <p> (stringToEl ("Round: " ^ string_of_int self.state.round)) </p>
        <p> (gameSeqToReactString self.state.seq) </p>
      </header>
      <hr />
      (
        switch self.state.gameState {
        | Displaying =>
          <SequenceDisplay
            displaySeq=self.state.seq
            timeoutDelay=500
            displayEndNotifier=(self.reduce (fun _ => DisplayComplete))
          />
        | Playing =>
          <SequenceInput inputModels=(buildInputModelArray numKeys self) />
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
