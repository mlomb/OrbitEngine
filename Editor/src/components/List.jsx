import React, { memo } from 'react';
import ReactDOM from 'react-dom';

import { FixedSizeList, areEqual } from 'react-window';
import AutoSizer from 'react-virtualized-auto-sizer';

import '@styles/List.less';

import Highlight from '@components/ui/Highlight.jsx';

/*
    itemHeight: number,
    data: [{ uid: string, title: string }, ...],
    renderRow: ({ row, titleElement }) => React.Component,
    customNavigation: (currentIndex: number, visible_items: [...], key: string) => number, // (the next index, -1 to default, null to discard)
    customFilter: (items: [...], pattern: RegExp) => [...]
*/
export default class List extends React.Component {

    state = {
        selection: null
    };

    constructor(props) {
        super(props);

        this.visible_items = null;

        this.containerRef = React.createRef();
        this.listRef = React.createRef();
    }

    filter(items, pattern) {
        if(!pattern)
            return items; // no filter
        return items.filter(x => pattern.test(x.title));
    }

    setSelection(uid) {
        if(this.state.selection === uid)
            return;

        this.setState({ selection: uid });
        this.containerRef.current.focus(); // re gain focus, may be lost to a child component re rendering
    }

    // Keyboard Navigation
    onKeyDown(e) {
        const isNavigationKey = e.keyCode >= 35 && e.keyCode <= 40; // arrows & home & end (deprecated keyCode?)
        const { visible_items } = this;

        if(isNavigationKey) {
            e.preventDefault();

            if(visible_items.length === 0)
                return;

            let selectionIndex = visible_items.findIndex(i => i.uid === this.state.selection);
            let newSelectionIndex = this.props.customNavigation ? this.props.customNavigation(selectionIndex, visible_items, e.key) : -1;

            if(newSelectionIndex === -1) {
                switch(e.key) {
                    case 'Home':
                        newSelectionIndex = 0; // first
                        break;
                    case 'End':
                        newSelectionIndex = visible_items.length - 1; // last
                        break;
                    case 'ArrowUp':
                    case 'ArrowDown':
                        newSelectionIndex = selectionIndex + (e.key === 'ArrowUp' ? -1 : 1);
                        break;
                    default:
                        newSelectionIndex = null;
                        break;
                }
            }

            if(newSelectionIndex === null)
                return; // discard

            newSelectionIndex = Math.max(0, Math.min(visible_items.length - 1, newSelectionIndex));

            this.setSelection(visible_items[newSelectionIndex].uid);
            this.listRef.current.scrollToItem(newSelectionIndex, "auto"); // scroll to the selected item
        }
    }

    // Deselection
    onMouseDown(e) {
        if(!e.target.closest('.row')) {
            const clickY = e.pageY - ReactDOM.findDOMNode(this.listRef.current).getBoundingClientRect().y;
            
            // avoid clicks on the scrollbar
            if(clickY > this.props.itemHeight * this.visible_items.length)
                this.setState({ selection: null });
        }
    }

    renderRow({ index, style }) {
        const row = this.visible_items[index];

        const isSelected = this.state.selection === row.uid;
        
        const titleElement = <Highlight text={row.title} pattern={this.props.searchPattern} />;
        const element =
            <div
                className={["row", isSelected ? "selected" : ""].join(' ')}
                style={{ ...style, height: this.props.itemHeight }}
                tabIndex={-1}
                title={row.title}
                onClick={() => this.setSelection(row.uid)}>
                {this.props.renderRow ? this.props.renderRow({ row, titleElement }) : titleElement}
            </div>;

        return element;
    }

    render() {
        // apply filter
        let pattern = null;
        if(this.props.searchPattern instanceof RegExp)
            pattern = this.props.searchPattern;
        else if(this.props.searchPattern) {
            // Escape RegExp Function: https://github.com/bvaughn/highlight-words-core/blob/eb170f8a78c7926b613e72733267f3243696113c/src/utils.js#L172
            pattern = new RegExp(this.props.searchPattern.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, '\\$&'), 'gi'); // case insensitive
        }

        this.visible_items = (this.props.customFilter || this.filter.bind(this))(this.props.data, pattern);

        return (
            <div
                ref={this.containerRef}
                className="list"
                tabIndex={0}
                onKeyDown={this.onKeyDown.bind(this)}
                onMouseDown={this.onMouseDown.bind(this)}>
                <AutoSizer>
                {({ width, height }) => (
                    <FixedSizeList
                        ref={this.listRef}
                        width={width}
                        height={height}
                        itemCount={this.visible_items.length}
                        itemSize={this.props.itemHeight}
                        itemKey={i => this.visible_items[i].uid}>
                        {memo(this.renderRow.bind(this), areEqual)}
                    </FixedSizeList>
                )}
                </AutoSizer>
            </div>
        );
    }
}
