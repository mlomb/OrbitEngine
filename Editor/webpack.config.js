const path = require('path');
const git = require('git-rev-sync');
const webpack = require('webpack');

const CleanWebpackPlugin = require('clean-webpack-plugin');
const TerserPlugin = require('terser-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const CopyPlugin = require('copy-webpack-plugin');

const dist = path.resolve(process.cwd(), 'dist');
const devMode = process.env.NODE_ENV !== 'production';
const target = process.env.TARGET;

const postcssPlugins = [
    //require('stylelint'),
    require('autoprefixer')
].concat(devMode ? [] : [
    require('cssnano')
]);

module.exports = (config) => ({
    target: target === 'web' ? 'web' : 'electron-renderer',
    node: target === 'web' ? {
        fs: 'empty',
        net: 'empty',
        tls: 'empty',
        child_process: 'empty',
        electron: 'empty',
        Buffer: false
    } : undefined,
	mode: devMode ? 'development' : 'production',
	devtool: devMode ? 'inline-source-map' : false,
	entry: {
        index: './src/index.jsx'
    },
	output: {
		path: dist,
		publicPath: target === 'web' ? '/' : '',
		filename: "static/[name]." + (devMode ? '[hash:5].js' : '[hash:16].js'),
	},
	module: {
		rules: [
			{
				test: /\.jsx?$/,
				exclude: [/node_modules/],
				use: {
					loader: 'babel-loader',
					options: {
                        presets: ['@babel/preset-env', '@babel/preset-react'],
                        plugins: ['@babel/plugin-proposal-class-properties']
					}
				}
			},
            {
                test: /\.(svg|png|jpe?g|gif|mp4)$/,
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
	optimization: {
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
		new webpack.DefinePlugin({
            "process.env.BUILD_HASH": JSON.stringify(git.short() + (git.isDirty() ? '-dirty' : '')),
            "process.env.BUILD_TIME": JSON.stringify(new Date()),
            "process.env.TARGET": JSON.stringify(target)
        }),
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
        new CopyPlugin([{ from: 'public', to: dist }].concat(target === 'web' ? [] : [
            /* For Electron */
            { from: 'src/electron', to: dist },
            { from: 'package.json', to: dist }
        ]))
    ],
	devServer: {
        host: '0.0.0.0',
        disableHostCheck: true,
        inline: true
	}
});
