import React, { memo } from 'react';
import ReactDOM from 'react-dom';

import { FixedSizeList, areEqual } from 'react-window';
import AutoSizer from 'react-virtualized-auto-sizer';
import isEqual from 'react-fast-compare';

import '@styles/List.less';

import Highlight from '@components/ui/Highlight.jsx';

const ListRow = memo((props) => {
    const { row, style, isSelected, highlightPattern } = props;
    const { setSelection, renderRow } = props;

    console.log("Rendered", row.uid);

    const titleElement = <Highlight text={row.title} pattern={highlightPattern} />;
    //titleElement = <input className="edit" type="text" value={row.title}/>;

    const element =
        <div
            className={["row", isSelected ? "selected" : ""].join(' ')}
            style={style}
            tabIndex={-1}
            title={row.title}
            onClick={() => setSelection(row.uid)}>
            {renderRow ? renderRow({ row, titleElement }) : titleElement}
        </div>;

    return element;

}, (prevProps, nextProps) => 
    isEqual(prevProps.style, nextProps.style) &&
    prevProps.rowAreEqual === nextProps.rowAreEqual && // ===
    (prevProps.rowAreEqual ? prevProps.rowAreEqual(prevProps, nextProps) : true) &&
    prevProps.row.uid === nextProps.row.uid &&
    prevProps.row.title === nextProps.row.title &&
    prevProps.isSelected === nextProps.isSelected &&
    prevProps.renderRow === nextProps.renderRow && // ===
    (prevProps.highlightPattern || 'null').toString() === (nextProps.highlightPattern || 'null').toString()
);

const ListRowWrapper = (props) => {
    const { items, selection, highlightPattern } = props.data;
    const { setSelection, renderRow, rowAreEqual } = props.data;
    const row = items[props.index];

    return <ListRow
        style={props.style}
        row={row}
        isSelected={selection === row.uid}
        highlightPattern={highlightPattern}

        setSelection={setSelection}
        renderRow={renderRow}
        rowAreEqual={rowAreEqual}
    />;
};


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
        this.pattern = null;

        this.containerRef = React.createRef();
        this.listRef = React.createRef();

        this.itemKey = i => this.visible_items[i].uid;
        this.setSelection_func = this.setSelection.bind(this);
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

    // Also gain focus
    onMouseUp() {
        this.containerRef.current.focus();
    }

    render() {
        // apply filter
        this.pattern = null;
        if(this.props.searchPattern instanceof RegExp) {
            this.pattern = this.props.searchPattern;
        } else if(this.props.searchPattern) {
            // Escape RegExp Function: https://github.com/bvaughn/highlight-words-core/blob/eb170f8a78c7926b613e72733267f3243696113c/src/utils.js#L172
            this.pattern = new RegExp('(' + this.props.searchPattern.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, '\\$&') + ')', 'gi'); // case insensitive
        }
        
        console.log("============= RENDERING LIST =============");
        

        this.visible_items = (this.props.customFilter || this.filter.bind(this))(this.props.data, this.pattern);

        return (
            <div
                ref={this.containerRef}
                className="list"
                tabIndex={0}
                onKeyDown={this.onKeyDown.bind(this)}
                onMouseDown={this.onMouseDown.bind(this)}
                onMouseUp={this.onMouseUp.bind(this)}>
                <AutoSizer>
                {({ width, height }) => (
                    <FixedSizeList
                        ref={this.listRef}
                        width={width}
                        height={height}
                        itemCount={this.visible_items.length}
                        itemSize={this.props.itemHeight}
                        itemKey={this.itemKey}
                        itemData={{
                            items: this.visible_items,
                            selection: this.state.selection,

                            setSelection: this.setSelection_func,
                            renderRow: this.props.renderRow,
                            rowAreEqual: this.props.rowAreEqual,
                        }}>
                        {ListRowWrapper}
                    </FixedSizeList>
                )}
                </AutoSizer>
            </div>
        );
    }
}
