import React from 'react';

import Icon from './ui/Icon.jsx';
import Button from './ui/Button.jsx';

import '@styles/WorkspaceSelector.less';

class RecentWorkspaces extends React.Component {
    render() {
        return (
            <div>
                <ul>
                    <li>A</li>
                    <li>B</li>
                    <li>C</li>
                </ul>
            </div>
        );
    }
}

export default class WorkspaceSelector extends React.Component {
    render() {
        return (
            <div className="workspace-selector">
                <div className="title"><Icon icon="folder" size={30} /><span>Open a Workspace</span></div>
                <Button>Select folder...</Button>
                <hr/>
                <RecentWorkspaces/>
            </div>
        );
    }
}