Run this project:

```
npm install
npm start
# in another tab
npm run build
```

After you see the webpack compilation succeed (the `npm run build` step), open up the nested html files in `src/*` (**no server needed!**). Then modify whichever file in `src` and refresh the page to see the changes.

# Simon

This is a very raw, very WIP version of the simple memory game [Simon](https://en.wikipedia.org/wiki/Simon_(game)).

It uses reason-react.

The main `Simon` module is a state machine that defines the game state + the actions needed to transition between;
it holds the core state of the game (the sequence) and builds it up as the state transitions.

It has three props, `maxRounds`, `numKeys` and `maxRetries` which can be used to configure the game.
TODO: build out a config component to kick the game off.

It has two children, the display and the inputs. I would like to just have the inputs, disable & highlight them when displaying, but this turns out to be more work than it's worth in React; mutating the classNames would seem to be the best way, but then I have to write some painful boilerplate. Anyway.

## Dev

WebPack has been added to - there is a dev server now, and an index file gets automatically built.

Same process as above, but use `npm run server` to run the Webpack dev server and have it auto update on change.

It is served on port 4000; modify the webpack config to change it, I just had something running on the default port at the time.

## Notes

It is not a simple thing to pass records between components, as you would with JS objects - in React, it is simple to pass an array of objects then use them to dynamically build up a set of components. Doesn't really work in RR - the records have to be typed, the inference will fall over if they're passed to another module as the other module knows absolutely nothing about them. Need to be careful.

Debugging without React dev tools: I can't access the props or state in the console, which means I need debugging code written inside the app - either `ReasonReact.UpdateWithSideEffects` with a `Js.log`, or more usefully, actually creating an area in a component that prints out useful things in the current state. Painful.

Atom IDE in combo with Merlin is _really_ good. Beta is extremely buggy as of now though - plays badly with Proton mode, and atm seems to not like the "f" key which is [f]un. VSCode integration is great, but the UI is /shrug and unconfigurable.
