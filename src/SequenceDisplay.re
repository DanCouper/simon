/* ---------------------------
 * Helpers
 * --------------------------- */
/* `ReasonReact.stringToElement` is a little verbose, so alias it.
 * NOTE this particular vagary get annoying quickly. */
let stringToEl = ReasonReact.stringToElement;

/* ReasonReact doesn't play nice with React Devtools. This is a pain
 * point; for development, manually printing out the state turns out
 * to be helpful: */
let intListToReactString intList =>
  stringToEl (Helpers.intListToString intList);

/* ---------------------------
 * SequenceDisplay component
 *
 * This, on mount, diplays a sequence of list items.
 * NOTE Initially, I tried to make this do too much - I included
 * the controls within the component, and tried to hook into
 * `didUpdate`, switching internal state. After a few failed
 * attempts, I used something that's basically identical to
 * the example in the guide.
 * TODO: replace `setinterval` with a `requestAnimationFrame`
 * based solution. Slight wierdness - Bs_WebIncubator has a
 * binding for `requestAnimationFrame`, but not for
 * `cancelAnimationFrame`.
 * TODO: this has to notify the parent that it's finished.
 * Need a callback passed as a prop.
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
  | [x, ...xs] => ReasonReact.Update {...state, displaySeq: xs}
  };

let component = ReasonReact.reducerComponent "SequenceDisplay";

let make ::displaySeq ::timeoutDelay=1000 _children => {
  ...component,
  initialState: fun () => {displaySeq: displaySeq, timerId: ref None},
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
