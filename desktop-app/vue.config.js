const path = require('path')

module.exports = {
  lintOnSave: false,
  pages: {
    index: {
      // entry for the page
      entry: 'src/main.js',
      // the source template
      template: 'public/index.html',
      // output as dist/index.html
      filename: 'index.html',
      // when using title option,
      // template title tag needs to be <title><%= htmlWebpackPlugin.options.title %></title>
      title: 'CanSat platform',
      // chunks to include on this page, by default includes
      // extracted common chunks and vendor chunks.
      chunks: ['chunk-vendors', 'chunk-common', 'index'],
    },
  },
  configureWebpack: {
    optimization: {
      namedChunks: true,
      namedModules: true,
      moduleIds: 'named',
      concatenateModules: false,
      minimize: false
    },
    output: {
      filename: '[name].[hash].bundle.js'
    },
    resolve: {
      alias: {
        'vue$': 'vue/dist/vue.esm.js',
        '@': path.resolve('src'),
        'src': path.resolve('src'),
        'assets': path.resolve('src/assets'),
        'components': path.resolve('src/components'),
        'services': path.resolve('src/services'),
        'directives': path.resolve('src/directives'),
        'vuestic-mixins': path.resolve('src/vuestic-theme/vuestic-mixins'),
        'vuestic-components': path.resolve('src/vuestic-theme/vuestic-components'),
        'vuestic-directives': path.resolve('src/vuestic-theme/vuestic-directives'),
        'vuestic-theme': path.resolve('src/vuestic-theme'),
        'data': path.resolve('src/data'),
        'vuex-store': path.resolve('src/store')
      }
    },
    externals: {
      bindings: "require('bindings')",
      serialport: "require('serialport')"
    }
  },
  css: {
    loaderOptions: {
      // pass options to sass-loader
      sass: {
        // @/ is an alias to src/
        data: `@import "@/sass/shared.scss";`
      }
    }
  },

}
