nnoremap <silent> <F9> :w<CR>:silent !gnome-terminal -- /bin/bash -c "make -s debug"<CR>
" nnoremap <silent> <F10> :w<CR>:silent !gnome-terminal -- /bin/bash -c "make -s run"<CR>

" nnoremap <silent> <F9> :w<CR>:!make -s debug<CR>
nnoremap <silent> <F10> :w<CR>:!make -s run<CR>
