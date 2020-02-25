import React, { memo } from 'react';
import ReactDOM from 'react-dom';

import { FixedSizeList, areEqual } from 'react-window';
import AutoSizer from 'react-virtualized-auto-sizer';

import '@styles/List.less';

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
        const isNavigationKey = e.keyCode >= 35 && e.keyCode <= 40; // arrows & home & end
        const { data } = this.props;

        if(isNavigationKey) {
            e.preventDefault();

            if(this.props.data.length === 0)
                return;

            let selectionIndex = this.props.data.findIndex(i => i.uid === this.state.selection);
            let newSelectionIndex = -1;
            
            switch(e.keyCode) {
                case 36: // Home
                    newSelectionIndex = 0; // first
                    break;
                case 35: // End
                    newSelectionIndex = this.props.data.length - 1; // last
                    break;
                case 37:
                case 38:
                case 39:
                case 40:
                    // arrows
                    newSelectionIndex = selectionIndex + (e.keyCode === 38 ? -1 : 1);
                    break;
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
        
        const element =
            <div
                className={["row", isSelected ? "selected" : ""].join(' ')}
                style={style}
                tabIndex={-1}
                onClick={() => this.setSelection(row.uid)}>
                {row.title}
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
