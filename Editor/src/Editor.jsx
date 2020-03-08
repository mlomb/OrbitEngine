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
import HeaderSearchBar from '@components/HeaderSearchBar.jsx';
import HierarchyTree from '@components/HierarchyTree.jsx';
import List from '@components/List.jsx';
import EngineService from '@service/EngineService';

var total_generated = 0;

class HierarchyTreeTest extends React.Component {
    state = {
        search_term: "",
        testTree: []
    };

    componentDidMount() {
        this.setState({ testTree: this.generateSampleData() });
        console.log(total_generated);
    }

    generateSampleData(depth = 1) {
        let to_generate = (depth == 1 ? 1 : 0) + Math.floor(Math.random() * 15);
        let result = [];
    
        for(let i = 0; i < to_generate; i++) {
            total_generated++;
            let uid = '' + Math.floor(Math.random() * 100000000);
            result.push({
                uid: uid,
                title: uid,
                children: (Math.random() > Math.pow(2, depth) / 128) ? this.generateSampleData(depth + 1) : []
            });
        }

        return result;
    }

    render() {
        return (
            <div style={{ width: '100%', height: '100%', display: 'flex', flexDirection: 'column' }}>
                <HeaderSearchBar searchTerm={this.state.search_term} setSearchTerm={(t) => this.setState({ search_term: t })} />
                <HierarchyTree data={this.state.testTree} searchPattern={this.state.search_term} />
            </div>
        );
    }
}


class ListTest extends React.Component {
    state = {
        testList: []
    };

    componentDidMount() {
        this.setState({ testList: this.generateSampleData() });
    }

    generateSampleData() {
        let result = [];
    
        for(let i = 0; i < 30; i++) {
            let uid = '' + Math.floor(Math.random() * 100000000);
            result.push({
                uid: uid,
                title: uid
            });
        }
    
        return result;
    }
    
    render() {
        return <List data={this.state.testList} itemHeight={25} />
    }
}

let tab = {
    content: <HierarchyTreeTest/>,
    cached: true,
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
        floatable: true,
        maximizable: false,
        tabLocked: false
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
              tabs: [{...tab, id: 't1', title: 'Hierarchy Tree'}],
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
          tabs: [{...tab, content: <ListTest/>, id: 't8', title: 'List'}],
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
            <div className={`page-container ${isElectron ? 'electron' : 'web'} ${isDevMode ? 'dev' : 'prod'}`}>
                <Menu/>
                <div className="main">
                    <DndProvider backend={Backend}>
                        <DockLayout dropMode="edge" groups={groups} defaultLayout={layout}/>
                    </DndProvider>
                </div>
                <StatusBar/>
            </div>
        );
    }
}

// {/*<WorkspaceSelector/>*/}