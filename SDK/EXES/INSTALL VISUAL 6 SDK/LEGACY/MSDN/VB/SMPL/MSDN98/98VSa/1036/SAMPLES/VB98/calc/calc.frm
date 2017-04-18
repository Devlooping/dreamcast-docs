VERSION 5.00
Begin VB.Form Calculator 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Calculatrice"
   ClientHeight    =   2970
   ClientLeft      =   2580
   ClientTop       =   2460
   ClientWidth     =   3225
   ClipControls    =   0   'False
   BeginProperty Font 
      Name            =   "System"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "CALC.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   2970
   ScaleWidth      =   3225
   WhatsThisHelp   =   -1  'True
   Begin VB.CommandButton Number 
      Caption         =   "7"
      Height          =   480
      Index           =   7
      Left            =   120
      TabIndex        =   7
      Top             =   600
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "8"
      Height          =   480
      Index           =   8
      Left            =   720
      TabIndex        =   8
      Top             =   600
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "9"
      Height          =   480
      Index           =   9
      Left            =   1320
      TabIndex        =   9
      Top             =   600
      Width           =   480
   End
   Begin VB.CommandButton Cancel 
      Caption         =   "C"
      Height          =   480
      Left            =   2040
      TabIndex        =   10
      Top             =   600
      Width           =   480
   End
   Begin VB.CommandButton CancelEntry 
      Caption         =   "CE"
      Height          =   480
      Left            =   2640
      TabIndex        =   11
      Top             =   600
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "4"
      Height          =   480
      Index           =   4
      Left            =   120
      TabIndex        =   4
      Top             =   1200
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "5"
      Height          =   480
      Index           =   5
      Left            =   720
      TabIndex        =   5
      Top             =   1200
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "6"
      Height          =   480
      Index           =   6
      Left            =   1320
      TabIndex        =   6
      Top             =   1200
      Width           =   480
   End
   Begin VB.CommandButton Operator 
      Caption         =   "+"
      Height          =   480
      Index           =   1
      Left            =   2040
      TabIndex        =   12
      Top             =   1200
      Width           =   480
   End
   Begin VB.CommandButton Operator 
      Caption         =   "-"
      Height          =   480
      Index           =   3
      Left            =   2640
      TabIndex        =   13
      Top             =   1200
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "1"
      Height          =   480
      Index           =   1
      Left            =   120
      TabIndex        =   1
      Top             =   1800
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "2"
      Height          =   480
      Index           =   2
      Left            =   720
      TabIndex        =   2
      Top             =   1800
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "3"
      Height          =   480
      Index           =   3
      Left            =   1320
      TabIndex        =   3
      Top             =   1800
      Width           =   480
   End
   Begin VB.CommandButton Operator 
      Caption         =   "X"
      Height          =   480
      Index           =   2
      Left            =   2040
      TabIndex        =   14
      Top             =   1800
      Width           =   480
   End
   Begin VB.CommandButton Operator 
      Caption         =   "/"
      Height          =   480
      Index           =   0
      Left            =   2640
      TabIndex        =   15
      Top             =   1800
      Width           =   480
   End
   Begin VB.CommandButton Number 
      Caption         =   "0"
      Height          =   480
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   2400
      Width           =   1080
   End
   Begin VB.CommandButton Decimal 
      Caption         =   "."
      Height          =   480
      Left            =   1320
      TabIndex        =   18
      Top             =   2400
      Width           =   480
   End
   Begin VB.CommandButton Operator 
      Caption         =   "="
      Height          =   480
      Index           =   4
      Left            =   2040
      TabIndex        =   16
      Top             =   2400
      Width           =   480
   End
   Begin VB.CommandButton Percent 
      Caption         =   "%"
      Height          =   480
      Left            =   2640
      TabIndex        =   17
      Top             =   2400
      Width           =   480
   End
   Begin VB.Label Readout 
      Alignment       =   1  'Right Justify
      BackColor       =   &H0000FFFF&
      BorderStyle     =   1  'Fixed Single
      Caption         =   "0,"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   375
      Left            =   120
      TabIndex        =   19
      Top             =   105
      Width           =   3000
   End
End
Attribute VB_Name = "Calculator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ----------------------------------------------------------------
'               Copyright (C) 1994 Microsoft Corporation
'
' Vous pouvez, librement et gratuitement, utiliser, modifier,
' copier et distribuer les fichiers des exemples d'application
' (et/ou toute version modifi�e) � condition que vous acceptiez le
' fait que Microsoft n'apporte aucune garantie ou obligation et
' n'engage aucune responsabilit� pour les fichiers des exemples
' d'application.
' ----------------------------------------------------------------
Option Explicit
Dim Op1, Op2                ' Op�rande saisi pr�c�demment.
Dim DecimalFlag As Integer  ' Le s�parateur d�cimal est-il d�j� en place?
Dim NumOps As Integer       ' Nombre d'op�randes.
Dim LastInput               ' Indique le type de la derni�re touche enfonc�e.
Dim OpFlag                  ' Indique quelle est l'op�ration en cours.
Dim TempReadout

' Proc�dure de l'�v�nement Click pour la touche C (Annuler).
' R�initialise l'affichage et initialise les variables.
Private Sub Cancel_Click()
    Readout = Format(0, "0,")
    Op1 = 0
    Op2 = 0
    Form_Load
End Sub

' Proc�dure de l'�v�nement Click pour la touche CE (Annuler la saisie).
Private Sub CancelEntry_Click()
    Readout = Format(0, "0,")
    DecimalFlag = False
    LastInput = "CE"
End Sub

' Proc�dure de l'�v�nement Click pour le s�parateur d�cimal (.).
' Si la derni�re touche enfonc�e �tait un op�rateur, initialise
' la valeur � "0," Sinon, ajoute un s�parateur d�cimal � l'�cran.
Private Sub Decimal_Click()
    If LastInput = "NEG" Then
        Readout = Format(0, "-,")
    ElseIf LastInput <> "NUMS" Then
        Readout = Format(0, ",")
    End If
    DecimalFlag = True
    LastInput = "NUMS"
End Sub

' Routine d'initialisation de la feuille.
' Affecte � toutes les variables leur valeur initiale.
Private Sub Form_Load()
    DecimalFlag = False
    NumOps = 0
    LastInput = "NONE"
    OpFlag = " "
    Readout = Format(0, "0,")
    'Decimal.Caption = Format(0, ",")
End Sub

' Proc�dure de l'�v�nement Click pour les touches num�riques (0-9).
' Ajoute un nouveau nombre au nombre affich�.
Private Sub Number_Click(Index As Integer)
    If LastInput <> "NUMS" Then
        Readout = Format(0, ",")
        DecimalFlag = False
    End If
    If DecimalFlag Then
        Readout = Readout + Number(Index).Caption
    Else
        Readout = Left(Readout, InStr(Readout, Format(0, ",")) - 1) + Number(Index).Caption + Format(0, ",")
    End If
    If LastInput = "NEG" Then Readout = "-" & Readout
    LastInput = "NUMS"
End Sub

' Proc�dure de l'�v�nement Click pour les touches d'op�rateurs (+, -, x, /, =).
' Si la touche pr�c�demment enfonc�e faisait partie d'un
' nombre, incr�mente NumOps. Si un op�rande est pr�sent,
' d�finit Op1. S'il y en a deux, affecte � Op1 une valeur
' �quivalente au r�sultat de l'op�ration sur Op1 et la
' cha�ne en cours de saisie, puis affiche le r�sultat.
Private Sub Operator_Click(Index As Integer)
    TempReadout = Readout
    If LastInput = "NUMS" Then
        NumOps = NumOps + 1
    End If
    Select Case NumOps
        Case 0
        If Operator(Index).Caption = "-" And LastInput <> "NEG" Then
            Readout = "-" & Readout
            LastInput = "NEG"
        End If
        Case 1
        Op1 = Readout
        If Operator(Index).Caption = "-" And LastInput <> "NUMS" And OpFlag <> "=" Then
            Readout = "-"
            LastInput = "NEG"
        End If
        Case 2
        Op2 = TempReadout
        Select Case OpFlag
            Case "+"
                Op1 = CDbl(Op1) + CDbl(Op2)
            Case "-"
                Op1 = CDbl(Op1) - CDbl(Op2)
            Case "X"
                Op1 = CDbl(Op1) * CDbl(Op2)
            Case "/"
                If Op2 = 0 Then
                   MsgBox "Division par z�ro.", 48, "Calculatrice"
                Else
                   Op1 = CDbl(Op1) / CDbl(Op2)
                End If
            Case "="
                Op1 = CDbl(Op2)
            Case "%"
                Op1 = CDbl(Op1) * CDbl(Op2)
            End Select
        Readout = Op1
        NumOps = 1
    End Select
    If LastInput <> "NEG" Then
        LastInput = "OPS"
        OpFlag = Operator(Index).Caption
    End If
End Sub

' Proc�dure de l'�v�nement Click pour la touche pourcentage (%).
' Calcule et affiche un pourcentage pour le premier op�rande.
Private Sub Percent_Click()
    Readout = Readout / 100
    LastInput = "Ops"
    OpFlag = "%"
    NumOps = NumOps + 1
    DecimalFlag = True
End Sub

