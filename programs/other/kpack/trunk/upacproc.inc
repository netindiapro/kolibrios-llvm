;*********************************************************************
unpack:
	call	displogo_and_readfile
	jnz	infileerr

	mov	ecx,[insize]
	test	ecx,ecx
	jz      infileerr

	mcall	68,12

	mov	[infile],eax

	mov	ebx,fn70block
	mov	[ebx],byte 0
	mov	[ebx+16],eax
	mov	eax,[insize]
	mov	[ebx+12],eax

	mcall	70
	test	eax,eax
	jnz	infileerr

	mov	eax,[infile]
	cmp	[eax],dword 'KPCK'
	jz	@f
;--------------------------------------
unpack_err:
	call	return_memory
	mov	esi,notpacked_str
	push	notpacked_len
	pop	ecx
	jmp	write_string
;---------------------------------------------------------------------
@@:
	mov	ecx,[eax+4]
	add	ecx,[insize]

	mcall	68,20,,[infile]

	mov	[infile],eax
	add	eax,[insize]
	mov	[outfile],eax
	mov	[outfilebest],eax


	mov	esi,[infile]
	mov	eax,[esi+8]
	push	eax
	and	al,0C0h
	cmp	al,0C0h
	pop	eax
	jz	unpack_err

	and	al,not 0C0h
	dec	eax
	jnz	unpack_err

	mov	eax,[esi+4]
	mov	[outsize],eax
	push	eax
	push	[outfile]
	add	esi,11
	push	esi
	mov	eax,[esi+1]
	xchg	al,ah
	ror	eax,16
	xchg	al,ah
	mov	[esi+1],eax
	call	lzma_decompress
	mov	esi,[infile]
	test	[esi+8],byte 80h
	jnz	uctr1

	test	[esi+8],byte 40h
	jz	udone

	add	esi,[insize]
	sub	esi,5
	lodsd
	mov	ecx,eax
	jecxz	udone

	mov	dl,[esi]
	mov	esi,[outfile]
;--------------------------------------
uc1:
	lodsb
	sub	al,0E8h
	cmp	al,1
	ja	uc1

	cmp	[esi],dl
	jnz	uc1

	lodsd
	shr	ax,8
	ror	eax,16
	xchg	al,ah
	sub	eax,esi
	add	eax,[outfile]
	mov	[esi-4],eax
	loop	uc1
	jmp	udone
;---------------------------------------------------------------------
uctr1:
	add	esi,[insize]
	sub	esi,5
	lodsd
	mov	ecx,eax
	jecxz	udone

	mov	dl,[esi]
	mov	esi,[outfile]
;--------------------------------------
uc2:
	lodsb
;--------------------------------------
@@:
	cmp	al,15
	jnz	uf

	lodsb
	cmp	al,80h
	jb	@b

	cmp	al,90h
	jb	@f
;--------------------------------------
uf:
	sub	al,0E8h
	cmp	al,1
	ja	uc2
;--------------------------------------
@@:
	cmp	[esi],dl
	jnz	uc2

	lodsd
	shr	ax,8
	ror	eax,16
	xchg	al,ah
	sub	eax,esi
	add	eax,[outfile]
	mov	[esi-4],eax
	loop	uc2
;--------------------------------------
udone:
	mov	esi,unpacked_ok
	push	unpacked_len
	pop	ecx
	call	write_string
	jmp	saveout
;*********************************************************************
return_memory:
	mov	ecx,[infile]
	test	ecx,ecx
	jz	@f
	mcall	68,13,
	xor	eax,eax
	mov	[infile],eax
@@:
	ret
;*********************************************************************
displogo_and_readfile:
	call	clear_mess_and_displogo
; load input file
	mov	esi,inname
	call	get_full_name
	mov	ebx,fn70block
	mov	[ebx],dword 5
	and	[ebx+4],dword 0
	and	[ebx+8],dword 0
	and	[ebx+12],dword 0
	mov	[ebx+16],dword file_attr
	mcall	70
	test	eax,eax
	ret
;*********************************************************************