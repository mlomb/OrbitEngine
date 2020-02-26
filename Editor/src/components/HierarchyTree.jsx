import React from 'react';

import { AiFillCaretRight, AiOutlineCaretDown } from 'react-icons/ai';
import { useDrag, useDrop } from 'react-dnd';

import '@styles/HierarchyTree.less';

import Highlight from '@components/ui/Highlight.jsx';
import List from '@components/List.jsx';

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
        expanded: []
    };

    constructor(props) {
        super(props);
        
        this.treeRef = React.createRef();
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

    customNavigation(currentIndex, visible_items, key) {
        const row = visible_items[currentIndex];

        if(!row)
            return -1;

        if(key === 'ArrowLeft') {
            if(row.children.length > 0 && row.expanded) {
                // collapse
                this.setExpanded(row.uid, false);
                return null;
            } else {
                // set selection to parent
                return visible_items.findIndex(i => i.uid === row.parent);
            }
        } else if(key === 'ArrowRight') {
            if(row.children.length > 0 && !row.expanded) {
                // expand
                this.setExpanded(row.uid, true);
                return null;
            } else {
                // set selection to next row with children.length > 0
                currentIndex++;
                while(currentIndex < visible_items.length) {
                    if(visible_items[currentIndex].children.length > 0)
                        return currentIndex;
                    currentIndex++;
                }
            }
            return null;
        }

        return -1; // fallback default
    }

    // Note: this function should run fast
    customFilter(data, pattern) {
        let flat_tree_data = []; // expanded items
        
        const isSearching = pattern instanceof RegExp;
        let pending = [];

        // sacar afuera para la optimizacion jit
        const DFS = (items, parent) => {
            for(let item of items) {
                // fill common data
                item.depth = parent ? parent.depth + 1 : 0;
                item.parent = parent ? parent.uid : null;
                item.expanded = isSearching || this.state.expanded.includes(item.uid);
                item.children = item.children || [];

                const shouldBeVisible = isSearching ? pattern.test(item.title) : true;

                if(shouldBeVisible) {
                    if(isSearching) {
                        flat_tree_data.push(...pending);
                        pending = [];
                    }
                    flat_tree_data.push(item);
                } else {
                    pending.push(item);
                }

                if(item.children.length > 0 && item.expanded) {
                    DFS(item.children, item);
                }

                pending.pop();
            }
        };

        DFS(data, null);

        return flat_tree_data;
    }

    render() {
        return (
            <div className="hierarchy-tree">
                <List
                    data={this.props.data}
                    itemHeight={25}
                    renderRow={this.renderRow.bind(this)}
                    customNavigation={this.customNavigation.bind(this)}
                    customFilter={this.customFilter.bind(this)}
                    searchPattern={this.props.searchPattern}
                    />
            </div>
        );
    }
}
