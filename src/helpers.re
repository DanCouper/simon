let intListToString (intList: list int) :string =>
  "[" ^ (List.map string_of_int intList |> String.concat ", ") ^ "]";

let justHead (list: list 'a) :option 'a =>
  switch list {
  | [] => None
  | [x, ..._xs] => Some x
  };

let dropHead (list: list 'a) :list 'a =>
  switch list {
  | [] => []
  | [_x, ...xs] => xs
  };
