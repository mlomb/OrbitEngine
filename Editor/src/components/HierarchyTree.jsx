import React from 'react';
//import { FixedSizeList as List } from "react-window";
import List from './List.jsx';
import AutoSizer from "react-virtualized-auto-sizer";
import { AiFillCaretRight, AiOutlineCaretDown, AiOutlineSearch, AiOutlinePlus } from 'react-icons/ai';
import { useDrag, useDrop } from 'react-dnd';

import '@styles/HierarchyTree.less';

import Highlight from '@components/ui/Highlight.jsx';

/*
    data format

    [
        {
            uid: string,
            title: string,
            children: [{ ... }, ...]
        },
        ...
    ]
*/

function traverse(items, fn, parent = null, depth = 0) {
    if(!items)
        return;
    for(let item of items) {
        if(fn(item, parent, depth))
            traverse(item.children, fn, item, depth + 1);
    }
}

function flatten(into, node){
    if(node == null) return into;
    if(Array.isArray(node)) return node.reduce(flatten, into);
    into.push(node);
    return flatten(into, node.children);
}

function isParent(flat_tree_data, parent_uid, child_uid) {
    let i = 0;
    let parent_depth = 0;

    // Step 1: find the index of the parent
    for(; i < flat_tree_data.length; i++) {
        let row = flat_tree_data[i];
        if(row.uid === child_uid)
            return false; // can't be the parent if it appears before
        else if(row.uid === parent_uid) {
            // found the parent
            parent_depth = row.depth;
            break;
        }
    }
    i++;

    // Step 2: from i, find the children before the depths are incompatible
    while(i < flat_tree_data.length) {
        let row = flat_tree_data[i];
        if(row.depth <= parent_depth)
            return false; // depth no longer compatible and we didn't found the child
        if(row.uid === child_uid)
            return true;
        i++;
    }

    return false; // not found
}


const TreeRow = () => {


    /*
    const [{ isDragging }, drag] = useDrag({
        item: { uid: row.uid, depth: row.depth, type: 'box' },
        end: (item, monitor) => {
            const dropResult = monitor.getDropResult()
            if (item && dropResult) {
                alert(`${item.uid} ---> ${dropResult.uid}`)
            }
        },
        collect: monitor => ({
            isDragging: monitor.isDragging(),
        }),
    })

    const [{ isOver, canDrop }, drop] = useDrop({
        accept: 'box',
        drop: () => row,
        canDrop: item => item.uid !== row.uid && !isParent(item.uid, row.uid),
        collect: monitor => ({
            isOver: monitor.isOver(),
            canDrop: monitor.canDrop()
        }),
    })
    drag(drop(
    */
};

/*
    data: [{ uid: string, title: string, children: [...] }, ...],
*/
export default class HierarchyTree extends React.Component {

    state = {
        expanded: [],

        search_term: ""
    };

    constructor(props) {
        super(props);
        
        this.treeRef = React.createRef();
        this.searchInputRef = React.createRef();

        this.setSearchTerm_func = this.setSearchTerm.bind(this);

        this.flat_tree_data = [];
    }

    onMouseDown(e) {
        let clickedRowElement = e.target.closest('.tree .row');
        let rowUID = null;

        let new_state = { };

        if(clickedRowElement) {
            rowUID = clickedRowElement.getAttribute('data-uid');
        }

        let clickedOnCaret = e.target.closest('.caret') !== null;
        if(clickedOnCaret) {
            // expand/contract
        } else {
            new_state.selection = rowUID;
        }

        this.setState(new_state, () => {
            // focus onMouseDown
            //setTimeout(() => this.treeRef.current.focus()); // must be done in the next tick
        });
    }

    setExpanded(uid, expanded) {
        let new_expanded;
        if(expanded) {
            new_expanded = this.state.expanded;
            new_expanded.push(uid);
        } else {
            new_expanded = this.state.expanded.filter(x => x !== uid);
        }
        this.setState({ expanded: new_expanded });
    }

    setSearchTerm(e) {
        this.setState({ search_term: e.target.value });
    }

    renderRow({ row, titleElement }) {
        return (
            <div className="hierarchy-row" style={{ paddingLeft: 3 + 20 * row.depth }}
                >
                <div className="caret" onClick={() => this.setExpanded(row.uid, !row.expanded)}>
                    {row.children.length > 0 ? (row.expanded ? <AiOutlineCaretDown /> : <AiFillCaretRight />) : null}
                </div>
                {titleElement}
            </div>
        );
    }

    customNavigation(currentIndex, row, key) {
        if(!row)
            return -1;

        if(key === 'ArrowLeft') {
            if(row.children.length > 0 && row.expanded) {
                // collapse
                this.setExpanded(row.uid, false);
                return null;
            } else {
                // set selection to parent
                return this.flat_tree_data.findIndex(i => i.uid === row.parent);
            }
        } else if(key === 'ArrowRight') {
            if(row.children.length > 0 && !row.expanded) {
                // expand
                this.setExpanded(row.uid, true);
                return null;
            } else {
                // set selection to next row with children.length > 0
                currentIndex++;
                while(currentIndex < this.flat_tree_data.length) {
                    if(this.flat_tree_data[currentIndex].children.length > 0)
                        return currentIndex;
                    currentIndex++;
                }
            }
            return null;
        }

        return -1; // fallback default
    }

    render() {
        this.flat_tree_data = []; // expanded items

        traverse(this.props.data, (item, parent, depth) => {
            item.depth = depth;
            item.expanded = this.state.expanded.includes(item.uid);
            item.parent = parent ? parent.uid : null;
            
            if(this.state.search_term.length > 0) {
                if(item.title.includes(this.state.search_term)) {
                    this.flat_tree_data.push(item);
                    return true;
                } else {
                    return true;
                }
            }
            this.flat_tree_data.push(item);
            return item.expanded;
        });

        const isParent_func = (p, c) => isParent(this.flat_tree_data, p, c);
        const searchPattern = this.state.search_term.length > 0 ? this.state.search_term : null;

        return (
            <div className="hierarchy-tree">
                <div className="top-bar">
                    <AiOutlineSearch className="icon" onClick={() => this.searchInputRef.current.focus()} />
                    <input type="text" placeholder="Search..." ref={this.searchInputRef} value={this.state.search_term} onChange={this.setSearchTerm_func} />
                    <div className="add">
                        <AiOutlinePlus/>
                    </div>
                </div>
                <List data={this.flat_tree_data} itemHeight={25} renderRow={this.renderRow.bind(this)} customNavigation={this.customNavigation.bind(this)} />
            </div>
        );
    }
}
