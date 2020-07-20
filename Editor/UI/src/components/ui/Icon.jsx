import React from 'react';

import '@styles/Icons.less';

export default (props) =>
    <i style={{ width: props.size, height: props.size, ...props.style }} className={`${props.icon}-icon`}/>