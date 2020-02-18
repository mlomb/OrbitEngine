import React from 'react';
import DockLayout from 'rc-dock';

import 'rc-dock/style/index.less';
import '@styles/dock.less';
import '@styles/Editor.less';

import { isDevMode, isElectron } from './ApplicationContext';

import Menu from '@components/Menu.jsx';
import StatusBar from '@components/StatusBar.jsx';
import RenderOutput from '@components/RenderOutput.jsx';
import WorkspaceSelector from '@components/WorkspaceSelector.jsx';
import Button from '@components/ui/Button.jsx';
import EngineService from '@service/EngineService';

let tab = {
    content: <div>Tab Content</div>,
    closable: false,
    group: 'normal'
};
let groups = {
    main: {
        floatable: false,
        maximizable: true,
        tabLocked: true
    },
    normal: {
        floatable: false,
        maximizable: false,
        tabLocked: true
    }
}
  
  let layout = {
    dockbox: {
      mode: 'horizontal',
      children: [
        {
          mode: 'vertical',
          size: 200,
          children: [
            {
              tabs: [{...tab, id: 't1', title: 'Tab 1'}, {...tab, id: 't2', title: 'Tab 2'}],
            },
            {
              tabs: [{
                ...tab, id: 't3', title: 'Min Size', content: (
                  <div>
                    <p>This tab has a minimal size</p>
                    150 x 150 px
                  </div>
                ), minWidth: 150, minHeight: 150,
              }, {...tab, id: 't4', title: 'Tab 4'}],
            },
          ]
        },
        {
          size: 1000,
          tabs: [
            {
              ...tab, id: 't5', title: 'basic demo', content: (
                  <RenderOutput/>
              ),
              closable: false,
              group: 'main'
            }
          ]
        },
        {
          size: 200,
          tabs: [{...tab, id: 't8', title: 'Tab 8'}],
        },
      ]
    }
  }
;
export default class Editor extends React.Component {

    componentDidMount() {
        EngineService.boot();
    }

    render() {
        return (
            <div className={`page-container ${isElectron() ? 'electron' : 'web'} ${isDevMode() ? 'dev' : 'prod'}`}>
                <Menu/>
                <div className="main">
                    {<DockLayout dropMode="edge" groups={groups} defaultLayout={layout}/>}
                    {/*<WorkspaceSelector/>*/}
                </div>
                <StatusBar/>
            </div>
        );
    }
}