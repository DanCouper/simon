type inputModel = Simon.inputModel;

let component = ReasonReact.statelessComponent "SequenceInput";

let make ::inputModels _children => {
  ...component,
  render: fun _self =>
    <figure>
      (
        ReasonReact.arrayToElement (
          Array.map
            (
              fun input =>
                <button
                  value=(string_of_int input.key)
                  key=(string_of_int input.key)
                  onClick=input.callback>
                  (ReasonReact.stringToElement (string_of_int input.key))
                </button>
            )
            inputModels
        )
      )
    </figure>
};
