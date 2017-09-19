let component = ReasonReact.statelessComponent "Simon";

let make _children => {
  ...component,
  render: fun _self =>
    <figure>
      <SequenceDisplay seq=[1, 2, 3, 4, 5, 6, 7] timeoutDelay=500 />
    </figure>
};
