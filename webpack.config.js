const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  entry: path.join(__dirname, '/lib/js/src/app.js'),
  output: {
    path: path.join(__dirname, '/dist'),
    filename: 'app.js'
  },
  devServer: {
    contentBase: path.join(__dirname, "/dist"),
    inline: true,
    port: 4000
  },
  plugins: [new HtmlWebpackPlugin({
    template: path.join(__dirname, '/src/index.ejs')
  })]
};
