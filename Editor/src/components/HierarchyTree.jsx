import React, { Children, cloneElement } from 'react';

import { AiFillCaretRight, AiOutlineCaretDown } from 'react-icons/ai';
import { FixedSizeList as List } from "react-window";
import AutoSizer from "react-virtualized-auto-sizer";

import '@styles/HierarchyTree.less';

/*
    [{
        title: string,
        expanded: boolean,
        children: [{ ... }, ...]
    }, ...]
*/
function traverse(items, fn, depth = 0) {
    if(!items)
        return;
    for(let item of items) {
        if(fn(item, depth))
            traverse(item.children, fn, depth + 1);
    }
}

const TreeItem = ({ item, isSelected, style, setExpansion, setSelection }) => {
    return (
        <div
            style={{
                ...style,
                paddingLeft: 3 + 20 * item.depth,
                paddingRight: 3
            }}
            className={["row", isSelected ? "selected" : ""].join(' ')}
            onClick={() => setSelection(item.uid)}
            >
            <div className="caret" onClickCapture={item.children.length > 0 ? (e) => { e.preventDefault(); e.stopPropagation(); setExpansion(item.uid, !item.expanded); } : null}>
                {item.children.length > 0 ? (item.expanded ? <AiOutlineCaretDown /> : <AiFillCaretRight />) : null}
            </div>
            <span>{item.title}</span>
        </div>
    );
};

export default class HierarchyTree extends React.Component {

    state = {
        expanded: [],
        selection: null
    };

    constructor(props) {
        super(props);

        this.setExpansion_func = this.setExpansion.bind(this);
        this.setSelection_func = this.setSelection.bind(this);
    }

    setExpansion(uid, expanded) {
        let new_expanded = this.state.expanded;
        if(expanded)
            new_expanded.push(uid);
        else {
            let index = new_expanded.indexOf(uid);
            if(index > -1)
                new_expanded.splice(index, 1);
        }

        this.setState({
            expanded: new_expanded
        });
    }

    setSelection(uid) {
        this.setState({ selection: uid });
    }

    tryResetSelection(e) {
        // only reset selection when the empty part of the tree is clicked
        if(e.target.parentElement.parentElement === e.currentTarget)
            this.setSelection(null);
    }

    render() {
        let flat_items = []; // expanded items
        traverse(this.props.data, (item, depth) => {
            item.depth = depth;
            item.expanded = this.state.expanded.includes(item.uid);

            flat_items.push(item);
            return item.expanded;
        });

        return (
            <div className="tree" onClick={this.tryResetSelection.bind(this)}>
                <AutoSizer>
                {({ height, width }) => (
                    <List
                        width={width}
                        height={height}
                        itemCount={flat_items.length}
                        itemSize={25}
                        itemKey={(i) => flat_items[i].uid}
                        >
                        {({ index, style }) => (
                            <TreeItem
                                style={style}
                                item={flat_items[index]}
                                isSelected={this.state.selection === flat_items[index].uid}
                                setExpansion={this.setExpansion_func}
                                setSelection={this.setSelection_func}
                            />
                        )}
                    </List>
                    )}
                </AutoSizer>
            </div>
        );
    }
}
