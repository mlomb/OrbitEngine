import React from 'react';
import DockLayout from 'rc-dock';
import Backend from 'react-dnd-html5-backend'
import { DndProvider } from 'react-dnd'

import 'rc-dock/style/index.less';
import '@styles/dock.less';
import '@styles/Editor.less';

import { isDevMode, isElectron } from './ApplicationContext';

import Menu from '@components/Menu.jsx';
import StatusBar from '@components/StatusBar.jsx';
import RenderOutput from '@components/RenderOutput.jsx';
import WorkspaceSelector from '@components/WorkspaceSelector.jsx';
import Button from '@components/ui/Button.jsx';
import HierarchyTree from '@components/HierarchyTree.jsx';
import EngineService from '@service/EngineService';

class Test extends React.Component {
    state = {
        testTree: []
    };

    componentDidMount() {
        this.setState({ testTree: this.generateSampleData() });
    }

    generateSampleData(depth = 1) {
        let to_generate = (depth == 1 ? 1 : 0) + Math.floor(Math.random() * 8);
        let result = [];
    
        for(let i = 0; i < to_generate; i++) {
            let uid = '' + Math.floor(Math.random() * 100000);
            result.push({
                uid: uid,
                title: uid,
                expanded: Math.random() > 0.5,
                //dragDisabled: false,
                children: (Math.random() > Math.pow(2, depth) / 64) ? this.generateSampleData(depth + 1) : []
            });
        }
    
        return result;
    }
    
    render() {
        return <HierarchyTree data={this.state.testTree} />
    }
}

let tab = {
    content: <Test/>,
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
                    <p>Tab Content</p>
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
              ...tab, id: 't5', title: 'Render Output', content: (
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
                    {
                    <DndProvider backend={Backend}>
                        <DockLayout dropMode="edge" groups={groups} defaultLayout={layout}/>}
                    </DndProvider>
                    }
                    {/*<WorkspaceSelector/>*/}
                </div>
                <StatusBar/>
            </div>
        );
    }
}