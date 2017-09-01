object Form1: TForm1
  Left = 285
  Top = 156
  BorderStyle = bsDialog
  Caption = 'Астероиды'
  ClientHeight = 400
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
  end
end
