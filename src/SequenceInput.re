type input = {keyValue: int};

let test = [{keyValue: 0}, {keyValue: 1}, {keyValue: 2}, {keyValue: 3}];

let component = ReasonReact.statelessComponent "SequenceInput";

let make ::inputs=test _children => {
  ...component,
  render: fun self =>
    <figure>
      (
        ReasonReact.arrayToElement (
          Array.of_list (
            List.map
              (
                fun {keyValue} =>
                  <button
                    value=(string_of_int keyValue)
                    key=(string_of_int keyValue)>
                    (ReasonReact.stringToElement (string_of_int keyValue))
                  </button>
              )
              inputs
          )
        )
      )
    </figure>
};
