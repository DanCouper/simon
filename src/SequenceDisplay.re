/* ---------------------------
 * Helpers
 * --------------------------- */
/* `ReasonReact.stringToElement` is a little verbose, so alias it: */
let stringToEl = ReasonReact.stringToElement;

/* ReasonReact doesn't play nice with React Devtools. This is a pain
 * point; for development, manually printing out the state turns out
 * to be helpful: */
let intListToReactString intList =>
  stringToEl (Helpers.intListToString intList);

/* ---------------------------
 * SequenceDisplay component
 * --------------------------- */
type action =
  | Tick;

type state = {
  displaySeq: list int,
  timerId: ref (option Js.Global.intervalId)
};

let sequence state =>
  switch state.displaySeq {
  | [] => ReasonReact.Update {...state, timerId: ref None}
  | _ => ReasonReact.Update {...state, displaySeq: Helpers.dropHead state.displaySeq}
  };

let component = ReasonReact.reducerComponent "SequenceDisplay";

let make ::seq ::timeoutDelay=1000 _children => {
  ...component,
  initialState: fun () => {displaySeq: seq, timerId: ref None},
  reducer: fun action state =>
    switch action {
    | Tick => sequence state
    },
  didMount: fun self => {
    self.state.timerId :=
      Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) timeoutDelay);
    ReasonReact.NoUpdate
  },
  render: fun {state} => <p> (intListToReactString state.displaySeq) </p>
};
