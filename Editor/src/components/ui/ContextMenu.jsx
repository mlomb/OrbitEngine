import React from 'react';

import '../../../assets/styles/ContextMenu.less';

import Icon from './Icon.jsx';

export default class ContextMenu extends React.Component {
    render() {
        return (
            <div className="context-menu">

<ul>
                <li><a href="#">A</a></li>
                <hr/>
                <li><a href="#">B</a></li>
                <li className="more">
                    <a href="#">C</a>
                    <ul>
                        <li><a href="#">D</a></li>
                        <li><a href="#">E</a></li>
                        <hr/>
                        <li className="more"><a href="#">F</a>
                    <ul>
                        <li><a href="#">G</a></li>
                        <li><a href="#">H</a></li>
                        <hr/>
                        <li><a href="#">I</a></li>
                        <li><a href="#">J</a></li>
                    </ul></li>
                        <li><a href="#">K</a></li>
                    </ul>
                </li>
                <li><a href="#">L</a></li>
            </ul>
            </div>
        );
    }
}