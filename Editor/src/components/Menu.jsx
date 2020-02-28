import React from 'react';
import { isElectron, getElectronWindow } from './../ApplicationContext';

import ContextMenu from './ui/ContextMenu.jsx';
import Icon from './ui/Icon.jsx';

export default class Menu extends React.Component {
    render() {
        return (
            <div className="menu">
                <div className="logo"></div>
                <ul className="items">
                    <li>
                        <a href="#">File</a>
                        <ContextMenu/>
                    </li>
                    <li>
                        <a href="#">Edit</a>
                        <ContextMenu/>
                    </li>
                    <li>
                        <a href="#">Selection</a>
                        <ContextMenu/>
                    </li>
                    <li>
                        <a href="#">View</a>
                        <ContextMenu/>
                    </li>
                    <li>
                        <a href="#">Help</a>
                        <ContextMenu/>
                    </li>
                </ul>
                {isElectron ?
                    <div className="win-buttons">
                        <button onClick={() => getElectronWindow().minimize()}><Icon icon="minimize"/></button>
                        <button onClick={() => getElectronWindow().isMaximized() ? getElectronWindow().unmaximize() : getElectronWindow().maximize() }><Icon icon="maximize"/></button>
                        <button onClick={() => getElectronWindow().close()} className="close">&times;</button>
                    </div>
                : null}
            </div>
        );
    }
}