import React, { memo } from 'react';
import ReactDOM from 'react-dom';

import { FixedSizeList, areEqual } from 'react-window';
import AutoSizer from 'react-virtualized-auto-sizer';

import '@styles/List.less';

/*
    itemHeight: number,
    data: [{ uid: string, title: string }, ...],
    renderRow: ({ row, titleElement }) => React.Component,
    customNavigation: (currentIndex, row, key) => number (the next index, -1 to default, null to discard)
*/
export default class List extends React.Component {

    state = {
        selection: null
    };

    constructor(props) {
        super(props);

        this.containerRef = React.createRef();
        this.listRef = React.createRef();
    }

    setSelection(uid) {
        this.setState({ selection: uid });
        this.containerRef.current.focus(); // re gain focus, may be lost to a child component re rendering
    }

    // Keyboard Navigation
    onKeyDown(e) {
        const isNavigationKey = e.keyCode >= 35 && e.keyCode <= 40; // arrows & home & end (deprecated keyCode?)
        const { data } = this.props;

        if(isNavigationKey) {
            e.preventDefault();

            if(this.props.data.length === 0)
                return;

            let selectionIndex = this.props.data.findIndex(i => i.uid === this.state.selection);
            let newSelectionIndex = this.props.customNavigation ? this.props.customNavigation(selectionIndex, this.props.data[selectionIndex], e.key) : -1;

            if(newSelectionIndex === null)
                return; // discard

            if(newSelectionIndex === -1) {
                switch(e.key) {
                    case 'Home':
                        newSelectionIndex = 0; // first
                        break;
                    case 'End':
                        newSelectionIndex = this.props.data.length - 1; // last
                        break;
                    case 'ArrowUp':
                    case 'ArrowDown':
                        newSelectionIndex = selectionIndex + (e.key === 'ArrowUp' ? -1 : 1);
                        break;
                }
            }

            newSelectionIndex = Math.max(0, Math.min(this.props.data.length - 1, newSelectionIndex));

            this.setSelection(data[newSelectionIndex].uid);
            this.listRef.current.scrollToItem(newSelectionIndex, "auto"); // scroll to the selected item
        }
    }

    // Deselection
    onMouseDown(e) {
        if(!e.target.closest('.row')) {
            const clickY = e.pageY - ReactDOM.findDOMNode(this.listRef.current).getBoundingClientRect().y;
            
            // avoid clicks on the scrollbar
            if(clickY > this.props.itemHeight * this.props.data.length)
                this.setState({ selection: null });
        }
    }

    renderRow({ index, style }) {
        const row = this.props.data[index];

        const isSelected = this.state.selection === row.uid;
        
        const titleElement = <span>{row.title}</span>;
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
        let data = this.props.data;

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
                        itemCount={data.length}
                        itemSize={this.props.itemHeight}
                        itemKey={i => data[i].uid}>
                        {memo(this.renderRow.bind(this), areEqual)}
                    </FixedSizeList>
                )}
                </AutoSizer>
            </div>
        );
    }
}
