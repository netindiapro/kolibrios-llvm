    mov  edi,params         ; ��������� �� ������
    call str_len            ; �������� ����� ������
    cmp  eax,0              ; ����������� ����� � ����
    je   red                ; ���� ���������� ��� �� ������
    call    _read_params    ; ����� ������ ��������� ����� (����� �����)
    xor  eax,eax            ; �������� �������
    mov  al,byte [params]   ; ������ �������� ����
    cmp  al,'N'             ; ���������� � normal mode
    jne _no_normalmode      ; ���� ��� �� ������ �� �������� ������
    mov  [runmode],1        ; ���� �� �� ������������� ���
    jmp  red                ; ��� �� ���������
  _no_normalmode:           ; ���� �� normal mode ��
    cmp  al,'H'             ; �������� hidden mode
    jne _no_hiddenmode      ; ���� �� �� �� ������ ������
    mov  [runmode],2        ; ���� �� �� ������������� ��� ��� �� (���?)
    call set_background     ; ������ ���
    jmp  bexit              ; � ��������� �����
  _no_hiddenmode:           ; ���� �� hidden mode
    cmp  al,'D'             ; �������� ��� color dialog mode
    jne _no_dialogmode      ; ���� �� ���� �� �� �� ������
    mov  [runmode],3        ; ����� ��������� ��� ��� ���� ��
    jmp  red                ; � ������ �� ����������
  _no_dialogmode:           ; ���� ��� �� ���� �� ��������� �����
    mov  [runmode],1        ; �� ���������� �� ������
    jmp  red                ; � ������