import React from 'react';

import '@styles/Button.less';

export default (props) =>
    <button className="btn" onClick={props.onClick}>{props.children}</button>