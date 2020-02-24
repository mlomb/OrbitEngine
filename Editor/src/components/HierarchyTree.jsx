import React from 'react';
import { FixedSizeList as List } from "react-window";
import AutoSizer from "react-virtualized-auto-sizer";
import { AiFillCaretRight, AiOutlineCaretDown, AiOutlineSearch, AiOutlinePlus } from 'react-icons/ai';
import { useDrag, useDrop } from 'react-dnd';

import '@styles/HierarchyTree.less';

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

function traverse(items, fn, depth = 0) {
    if(!items)
        return;
    for(let item of items) {
        if(fn(item, depth))
            traverse(item.children, fn, depth + 1);
    }
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


const TreeRow = ({ row, style, isSelected, isParent }) => {


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

    return drag(drop(
        <div
            style={{
                ...style,
                paddingLeft: 3 + 20 * row.depth,
                paddingRight: 3
            }}
            className={["row", isSelected ? "selected" : "", isOver && canDrop ? "can-drop" : "", isOver && !canDrop ? "cant-drop" : ""].join(' ')}
            data-uid={row.uid}
            tabIndex={0}
            >
            <div className="caret">
                {row.children.length > 0 ? (row.expanded ? <AiOutlineCaretDown /> : <AiFillCaretRight />) : null}
            </div>
            <span>{row.title}</span>
        </div>
    ));
};

export default class HierarchyTree extends React.Component {

    state = {
        focused: false,
        selection: null,
        expanded: []
    };

    constructor(props) {
        super(props);
        
        this.treeRef = React.createRef();
        this.searchInputRef = React.createRef();
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
            if(this.state.expanded.includes(rowUID)) {
                new_state.expanded = this.state.expanded.filter(x => x !== rowUID);
            } else {
                new_state.expanded = this.state.expanded;
                new_state.expanded.push(rowUID);
            }
        } else {
            new_state.selection = rowUID;
        }

        this.setState(new_state, () => {
            // focus onMouseDown
            //setTimeout(() => this.treeRef.current.focus()); // must be done in the next tick
        });
    }

    onFocus() {
        console.log("ON FOCUS");
        
        this.setState({ focused: true });
    }

    onBlur() {
        console.log("ON BLUR");

        this.setState({ focused: false });
    }

    render() {
        let flat_tree_data = []; // expanded items
        traverse(this.props.data, (item, depth) => {
            item.depth = depth;
            item.expanded = this.state.expanded.includes(item.uid);

            flat_tree_data.push(item);
            return item.expanded;
        });

        const isParent_func = (p, c) => isParent(flat_tree_data, p, c);

        return (
            <div className="hierarchy-tree">
                <div className="top-bar">
                    <AiOutlineSearch className="icon" onClick={() => this.searchInputRef.current.focus()} />
                    <input type="text" placeholder="Search..." ref={this.searchInputRef} />
                    <div className="add">
                        <AiOutlinePlus/>
                    </div>
                </div>
                <div className="tree"
                    ref={this.treeRef}
                    tabIndex={0}
                    onMouseDown={this.onMouseDown.bind(this)}
                    onFocus={this.onFocus.bind(this)}
                    onBlur={this.onBlur.bind(this)}
                    >
                    <AutoSizer>
                    {({ height, width }) => (
                        <List
                            width={width}
                            height={height}
                            itemCount={flat_tree_data.length}
                            itemSize={25}
                            itemKey={(i) => flat_tree_data[i].uid}
                            >
                            {({ index, style }) => (
                                <TreeRow
                                    style={style}
                                    row={flat_tree_data[index]}
                                    isSelected={this.state.selection === flat_tree_data[index].uid}
                                    isParent={isParent_func}
                                />
                            )}
                        </List>
                    )}
                    </AutoSizer>
                </div>
            </div>
        );
    }
}
