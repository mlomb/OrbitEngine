import React from 'react';
import ReactDOM from 'react-dom';

import Editor from './Editor.jsx';

import { isDevMode } from './ApplicationContext';

if (isDevMode) {
    const whyDidYouRender = require('@welldone-software/why-did-you-render');
    whyDidYouRender(React, {
        trackAllPureComponents: true,
        trackHooks: true,
        exclude: [/(Dock|DragDropDiv|FloatBox|MaxBox|Divider)/, /AutoSizer/]
    });
}

ReactDOM.render(<Editor/>, document.getElementById('app'));
