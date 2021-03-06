include 'proc32.inc'

macro start_draw_window x,y,xsize,ysize,areacolor,caption;,capsize
{
	mov	eax, 12 		; function 12:tell os about windowdraw
	mov	ebx, 1			; 1, start of draw
	int	0x40
	; DRAW WINDOW
	mov	eax, 48 		; function 48.4 : get skin height
	mov	ebx, 4
	int	0x40
	push	eax
	lea	ecx, [y*65536+ysize+eax]; [y start] *65536 + [y size] + [skin_height]
	xor	eax, eax		; function 0 : define and draw window
	mov	ebx, x*65536+xsize	; [x start] *65536 + [x size]
	mov	edx, areacolor		; color of work area RRGGBB
	;mov     esi, 0x00334455        ; color of grab bar  RRGGBB
	mov	edi, caption;0x00ddeeff          ; color of frames    RRGGBB
	int	0x40
	; WINDOW LABEL
	;mov     eax, 4                  ; function 4 : write text to window
	;mov     ebx, 8*65536+8          ; [x start] *65536 + [y start]
	;mov     ecx, 0x00ffffff         ; color of text RRGGBB
	;mov     edx, caption            ; pointer to text beginning
	;mov     esi, capsize    ; text length
	;mov     eax, 71                  ; function 71.1
	;mov     ebx, 1                   ; set window caption
	;mov     ecx, caption             ; pointer to text
	;int     0x40
	pop	eax
}

macro end_draw_window
{
	mov	eax, 12 		; end of redraw
	mov	ebx, 2
	int	0x40
}

proc draw_button stdcall, x:dword, y:dword, xsize:dword, ysize:dword, \
	id:dword, butcolor:dword, text:dword, textlen:byte, textcolor:dword

	;pusha
	mov	ebx, dword [x]
	shl	ebx, 16
	add	ebx, dword [xsize]	  ; [x start] *65536 + [x size]

	mov	ecx, dword [y]
	shl	ecx, 16
	add	ecx, dword [ysize]	  ; [y start] *65536 + [y size]

	mov	edx, dword [id] 		; button id
	mov	esi, dword [butcolor]		; button color RRGGBB
	mov	eax, 8			; function 8 : define and draw button
	int	0x40

	mov	ebx, dword [x]
	add	ebx, 5
	shl	ebx, 16
	mov	eax, dword [ysize]
	sub	eax, 5
	shr	eax, 1
	add	ebx, eax
	add	ebx, dword [y]	;mov    ebx, (x+5)*65536+y+(ysize-5)/2  ; Draw button text

	mov	ecx, dword [textcolor]
	mov	edx, dword [text]
	xor	eax, eax
	mov	al,  byte [textlen]
	mov	esi, eax
	mov	eax, 4
	int	0x40
	;popa
ret
endp

macro outtextxy x,y,prompt,prompt_len,color
{
	pusha
	mov	ebx, x*65536+y	       ; draw info text with function 4
	mov	ecx, color
	mov	edx, prompt
	xor	eax, eax
	mov	al,  prompt_len
	mov	esi, eax
	mov	eax, 4
	int	0x40
	popa
}

;proc bar x:dword, y:dword, xsize:dword, ysize:dword, color:dword
macro bar x, y, xsize, ysize, color
{
	pusha
	mov eax, 13
	;mov ebx, [x]
	;shl ebx, 16
	;add ebx, [xsize]
	;mov ecx, [y]
	;shl ecx, 16
	;add ecx, [ysize]
	;mov edx, [color]
	mov	ebx, x*65536+xsize
	mov	ecx, y*65536+ysize
	mov edx, color

	int 0x40
	popa
;ret
;endp
}

macro line x1,y1,x2,y2,color
{
  pusha
  mov eax, 38
  mov ebx, x1*65536+x2
  mov ecx, y1*65536+y2
  mov edx, color
  int 0x40
  popa
}

macro rectangle x,y,xsize,ysize,color
{
  x2=x+xsize
  y2=y+ysize
  line x,y,x2,y,color
  line x,y,x,y2,color
  line x,y2,x2,y2,color
  line x2,y,x2,y2,color
}

macro putpixel x,y,color
{
  mov eax, 1
  mov ebx, x
  mov ecx, y
  mov edx, color
  int 0x40
}

