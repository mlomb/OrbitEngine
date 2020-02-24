import React from 'react';
import reactStringReplace from 'react-string-replace';

import '@styles/Highlight.less';

export default (props) =>
    props.pattern ? reactStringReplace(props.text, props.pattern, (match, i) => <mark key={i}>{match}</mark>) : props.text;