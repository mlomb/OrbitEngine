const path = require('path');
const git = require('git-rev-sync');
const webpack = require('webpack');

const CleanWebpackPlugin = require('clean-webpack-plugin');
const TerserPlugin = require('terser-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');

const dist = path.resolve(process.cwd(), 'dist');
const devMode = process.env.NODE_ENV !== 'production';

console.log("devMode:", devMode);

const postcssPlugins = [
    //require('stylelint'),
    require('autoprefixer')
].concat(devMode ? [] : [
    require('cssnano')
]);

module.exports = (config) => ({
    target: 'web',
    node: {
        fs: 'empty',
        net: 'empty',
        tls: 'empty',
        __dirname: true,
        child_process: 'empty',
        Buffer: false
    },
    mode: devMode ? 'development' : 'production',
    devtool: devMode ? 'inline-source-map' : false,
    entry: {
        index: path.resolve(__dirname, 'src/index.jsx')
    },
    output: {
        path: dist,
        publicPath: '/',
        filename: "static/[name]." + (devMode ? '[hash:5].js' : '[hash:16].js'),
    },
    resolve: {
        alias: {
            '@assets': path.resolve(__dirname, 'assets'),
            '@public': path.resolve(__dirname, 'public'),
            '@styles': path.resolve(__dirname, 'assets/styles'),
            '@src': path.resolve(__dirname, 'src'),
            '@service': path.resolve(__dirname, 'src/service'),
            '@components': path.resolve(__dirname, 'src/components'),

            '7zip': path.resolve(__dirname, 'node_modules/win-7zip/index.js')
        },
        extensions: [ '.ts', '.tsx', '.js', '.jsx' ]
    },
    module: {
        rules: [
            {
                test: /\.(ts|tsx|jsx|js)$/,
                exclude: [/node_modules/],
                use: {
                    loader: 'babel-loader',
                    options: {
                        presets: ['@babel/preset-env', '@babel/preset-react', '@babel/typescript'],
                        plugins: ['@babel/plugin-proposal-class-properties']
                    }
                }
            },
            {
                test: /\.(svg|png|jpe?g|gif|mp4|woff|woff2|eot|ttf)$/,
                loader: "url-loader",
                options: {
                    limit: 2048,
                    name: "static/[hash:8].[ext]",
                    esModule: false
                }
            },
            {
                test: /\.(c|le)ss$/, 
                use: [
                    'style-loader',
                    'css-loader',
                    {
                        loader: 'postcss-loader',
                        options: {
                            plugins: postcssPlugins
                        }
                    },
                    'less-loader'
                ]
            }
        ]
    },
    optimization: devMode ? { minimize: false } : {
        minimize: true,
        mangleWasmImports: true,
        minimizer: devMode ? [] : [
            new TerserPlugin({
                cache: true,
                parallel: true,
                sourceMap: devMode,
                extractComments: true,
                terserOptions: {
                    toplevel: true,
                    compress: {
                        passes: 3,
                        arguments: true,
                        booleans_as_integers: false,
                        pure_getters: true,
                        unsafe: true
                    },
                },
            })
        ]
    },
    plugins: [
        new CleanWebpackPlugin(dist),
        new HtmlWebpackPlugin({
            template: './assets/index.html',
            chunks: ['vendor', 'index'],
            showErrors: devMode,
            minify: devMode ? {} : {
                removeComments: true,
                collapseWhitespace: true,
                removeRedundantAttributes: true,
                useShortDoctype: true,
                removeEmptyAttributes: true,
                removeStyleLinkTypeAttributes: true,
                keepClosingSlash: true,
                minifyJS: true,
                minifyCSS: true,
                minifyURLs: true,
                useShortDoctype: true
            }
        }),
        new CopyPlugin([{
			from: 'public',
			to: dist
		}]),
        new webpack.DefinePlugin({
            "__ENV__.NODE_ENV": JSON.stringify(process.env.NODE_ENV),
            "__ENV__.BUILD_HASH": JSON.stringify(git.short() + (git.isDirty() ? '-dirty' : '')),
            "__ENV__.BUILD_TIME": JSON.stringify(new Date())
        })
    ],
    devServer: {
        host: '0.0.0.0',
        disableHostCheck: true,
        inline: true
    }
});
