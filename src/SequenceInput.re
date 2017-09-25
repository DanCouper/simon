/* ---------------------------
 * Helpers
 * --------------------------- */
/* `ReasonReact.stringToElement` is a little verbose, so alias it.
 * NOTE this particular vagary get annoying quickly. */
let stringToEl = ReasonReact.stringToElement;

/* ReasonReact doesn't play nice with React Devtools. This is a pain
 * point; for development, manually printing out the state turns out
 * to be helpful: */
let checkSeqToReactString seq =>
  stringToEl ("Current check sequence: " ^ Helpers.intListToString seq);

/* ---------------------------
 * SequenceDisplay component
 *
 * This, on mount, diplays a number of inputs corresponding
 * to the possible values avvailable in the Simon sequence.
 *
 * Initially, I tried to have the state logic in the parent;
 * it got too messy, so this gets passed the current sequence,
 * and allows a player to test against it.
 *
 * FIXME: it should take two callbacks - a `complete` and
 * a `fail`, which will be reducers passed from the main
 * component. Logic to follow.
 * --------------------------- */
type checkState =
  | Guessing
  | Complete;

let checkStateToReactStr checkState =>
  switch checkState {
  | Guessing => stringToEl "Check state: Guessing"
  | Complete => stringToEl "Check state: Complete"
  };

type state = {checkState, currentCheckSeq: list int};

type action =
  | Check int;

let component = ReasonReact.reducerComponent "SequenceInput";

let make ::keys ::checkSeq ::roundCompleteNotifier ::gameOverNotifier _children => {
  ...component,
  initialState: fun () => {checkState: Guessing, currentCheckSeq: checkSeq},
  reducer: fun action state =>
    switch action {
    | Check i =>
      switch state.currentCheckSeq {
      /* The update happens on the next 'tick', so if I switch on an empty
       * list, there'll need to be another click before the state transitions.
       * This will poss be useful as I want an intermediate "round over" state,
       * but switch on a length-1 list for now. */
      | [x] when i == x =>
        ReasonReact.UpdateWithSideEffects
          {currentCheckSeq: [], checkState: Complete} roundCompleteNotifier
      | [x, ...xs] when i == x =>
        ReasonReact.UpdateWithSideEffects
          {...state, currentCheckSeq: xs} (fun _ => Js.log "Guess correct")
      | [x, ..._xs] when i != x =>
        ReasonReact.UpdateWithSideEffects
          {checkState: Guessing, currentCheckSeq: checkSeq} gameOverNotifier
      | _ => ReasonReact.NoUpdate
      }
    },
  render: fun self =>
    <figure>
      <header>
        <p> (checkStateToReactStr self.state.checkState) </p>
        <p> (checkSeqToReactString self.state.currentCheckSeq) </p>
      </header>
      (
        ReasonReact.arrayToElement (
          Array.of_list (
            List.map
              (
                fun i =>
                  <button
                    value=(string_of_int i)
                    key=(string_of_int i)
                    onClick=(self.reduce (fun _ => Check i))>
                    (ReasonReact.stringToElement (string_of_int i))
                  </button>
              )
              (Helpers.buildRange keys)
          )
        )
      )
    </figure>
};
