import React, { useState, useRef } from 'react';

import { AiOutlineSearch, AiOutlinePlus } from 'react-icons/ai';

import '@styles/HeaderSearchBar.less';

export default (props) => {
    const searchInputRef = useRef(null);

    return (
        <div className="header-search-bar">
            <AiOutlineSearch className="icon" onClick={() => searchInputRef.current.focus()} />
            <input type="text" placeholder="Search..." ref={searchInputRef} value={props.searchTerm} onChange={(e) => props.setSearchTerm(e.target.value)} />
            <div className="add">
                <AiOutlinePlus/>
            </div>
        </div>
    )
}