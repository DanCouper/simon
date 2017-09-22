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

The main `Simon` module should eventually just be a state machine that defines the game state + the actions needed to transition between;
it holds the core state of the game (the sequence) and builds it up as the state transitions.

It has two props, `maxRounds` and `numKeys` - eventually there should be a config component above Simon, but atm this just gets passed in via the ReactDOM render call.

It has two children, the display and the inputs. I would like to just have the inputs, disable & highlight them when displaying, but this turns out
to be more work than it works in React; mutating the classNames would seem to be the best way, but then I have to write some painful boilerplate. Anyway.

## Dev

WebPack has been added to - there is a dev server now, and an index file gets automatically built.

Same process as above, but use `npm run server` to run the Webpack dev server and have it auto update on change.

It is served on port 4000; modify the webpack config to change it, I just had something running on the default port at the time.

## Notes

_(mainly in code atm, need to remember to write down issues that arise)_
