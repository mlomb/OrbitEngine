import React from 'react';

import '../../../assets/styles/Button.less';

export default (props) =>
    <button className="btn" onClick={props.onClick}>{props.children}</button>