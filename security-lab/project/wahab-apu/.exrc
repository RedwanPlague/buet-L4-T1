nnoremap <silent> <F9> :w<CR>:silent !gnome-terminal -- /bin/bash -c "make -s debug"<CR>
nnoremap <silent> <F10> :w<CR>:silent !gnome-terminal -- /bin/bash -c "make -s run; echo; read -rs -p 'Press Enter to continue'"<CR>
