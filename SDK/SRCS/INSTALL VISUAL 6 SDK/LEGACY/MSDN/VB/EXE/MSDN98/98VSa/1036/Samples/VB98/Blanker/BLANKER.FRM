VERSION 5.00
Begin VB.Form DemoForm 
   BackColor       =   &H00000000&
   Caption         =   "D�monstration Mise en veille"
   ClientHeight    =   3855
   ClientLeft      =   960
   ClientTop       =   2535
   ClientWidth     =   7470
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00000000&
   Icon            =   "BLANKER.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "Form1"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3855
   ScaleWidth      =   7470
   WhatsThisHelp   =   -1  'True
   Begin VB.Timer Timer1 
      Interval        =   1
      Left            =   6960
      Top             =   120
   End
   Begin VB.CommandButton cmdStartStop 
      BackColor       =   &H00000000&
      Caption         =   "Lancer la d�mo"
      Default         =   -1  'True
      Height          =   390
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1830
   End
   Begin VB.PictureBox picBall 
      AutoSize        =   -1  'True
      BackColor       =   &H00000000&
      BorderStyle     =   0  'None
      ForeColor       =   &H00FFFFFF&
      Height          =   480
      Left            =   1800
      Picture         =   "BLANKER.frx":030A
      ScaleHeight     =   480
      ScaleWidth      =   480
      TabIndex        =   1
      Top             =   720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   8
      Left            =   6330
      Picture         =   "BLANKER.frx":0614
      Top             =   3765
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Line linLineCtl 
      BorderColor     =   &H00FF0000&
      BorderWidth     =   5
      Visible         =   0   'False
      X1              =   240
      X2              =   4080
      Y1              =   2760
      Y2              =   2760
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   7
      Left            =   5760
      Picture         =   "BLANKER.frx":091E
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   6
      Left            =   5160
      Picture         =   "BLANKER.frx":0C28
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   5
      Left            =   4560
      Picture         =   "BLANKER.frx":0F32
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   4
      Left            =   3960
      Picture         =   "BLANKER.frx":123C
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   3
      Left            =   3360
      Picture         =   "BLANKER.frx":1546
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   2
      Left            =   2760
      Picture         =   "BLANKER.frx":1850
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   1
      Left            =   2160
      Picture         =   "BLANKER.frx":1B5A
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgMoon 
      Height          =   480
      Index           =   0
      Left            =   1560
      Picture         =   "BLANKER.frx":1E64
      Top             =   3720
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Shape shpClone 
      BackColor       =   &H00000000&
      BackStyle       =   1  'Opaque
      BorderColor     =   &H00FF0000&
      FillColor       =   &H000000FF&
      Height          =   1215
      Index           =   0
      Left            =   240
      Top             =   720
      Visible         =   0   'False
      Width           =   1410
   End
   Begin VB.Shape Shape1 
      Height          =   15
      Left            =   960
      Top             =   1080
      Width           =   15
   End
   Begin VB.Menu mnuOption 
      Caption         =   "&Options"
      Begin VB.Menu mnuLineCtlDemo 
         Caption         =   "&Ligne sautante"
         Checked         =   -1  'True
      End
      Begin VB.Menu mnuCtlMoveDemo 
         Caption         =   "Re&bond"
      End
      Begin VB.Menu mnuImageDemo 
         Caption         =   "&Rotation de la lune"
      End
      Begin VB.Menu mnuShapeDemo 
         Caption         =   "&Objets de couleur"
      End
      Begin VB.Menu mnuPSetDemo 
         Caption         =   "&Confetti"
      End
      Begin VB.Menu mnuLineDemo 
         Caption         =   "B&alles tra�antes"
      End
      Begin VB.Menu mnuCircleDemo 
         Caption         =   "&Tapis arc-en-ciel"
      End
      Begin VB.Menu mnuScaleDemo 
         Caption         =   "Barr&es color�es"
      End
      Begin VB.Menu sep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExit 
         Caption         =   "&Quitter"
      End
   End
End
Attribute VB_Name = "DemoForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' D�clare une variable pour le suivi du cadre d'animation.
Dim FrameNum
' D�clare les variables des coordonn�es X et Y pour le suivi de la position.
Dim XPos
Dim YPos
' D�clare un indicateur de variable pour arr�ter les routines graphiques.
Dim DoFlag
' D�clare une variable pour suivre les contr�les en d�placement.
Dim Motion
' D�clare les variables de feuille pour la couleur.
Dim R
Dim G
Dim B

Private Sub CircleDemo()
    ' D�clare les variables locales.
    Dim Radius
    ' Cr�e des couleurs RGB au hasard.
    R = 255 * Rnd
    G = 255 * Rnd
    B = 255 * Rnd
    ' Positionne le centre des cercles au centre de la feuille.
    XPos = ScaleWidth / 2
    YPos = ScaleHeight / 2
    ' G�n�re un rayon compris entre 0 et pr�s de la moiti� de la hauteur de la feuille.
    Radius = ((YPos * 0.9) + 1) * Rnd
    ' Dessine un cercle sur la feuille.
    Circle (XPos, YPos), Radius, RGB(R, G, B)
End Sub

Private Sub cmdStartStop_Click()
' D�clare des variables locales.
Dim UnClone
Dim MakeClone
Dim X1
Dim Y1
    Select Case DoFlag
        Case True
            cmdStartStop.Caption = "Lancer la d�mo"
            DoFlag = False
            mnuOption.Enabled = True
            If mnuCtlMoveDemo.Checked = True Then
                ' Cache � nouveau le graphisme bondissant.
                picBall.Visible = False
            ElseIf mnuLineDemo.Checked = True Then
                ' Supprime des lignes de la feuille..
                Cls
            ElseIf mnuShapeDemo.Checked = True Then
                ' Supprime tous les contr�les Shape charg�s dynamiquement.
                For UnClone = 1 To 20
                    Unload shpClone(UnClone)
                Next UnClone
                ' R�initialise la couleur d'arri�re-plan � noir.
                DemoForm.BackColor = QBColor(0)
                ' Rafra�chit la feuille pour que le changement de couleur prenne effet.
                Refresh
            ElseIf mnuPSetDemo.Checked = True Then
                ' Supprime des morceaux de confettis de la feuille.
                Cls
            ElseIf mnuLineCtlDemo.Checked = True Then
                ' Cache le contr�le Line � nouveau.
                linLineCtl.Visible = False
                ' Supprime tous les pixels restants.
                Cls
            ElseIf mnuImageDemo.Checked = True Then
                ' Cache � nouveau le graphisme bondissant.
                imgMoon(0).Visible = False
            ElseIf mnuScaleDemo.Checked = True Then
                ' Efface la feuille.
                Cls
                ' Renvoie l'�chelle par d�faut de la feuille.
                Scale
            ElseIf mnuCircleDemo.Checked = True Then
                ' Supprime les cercles de la feuille.
                Cls
            End If
        Case False
            cmdStartStop.Caption = "Stopper la d�mo"
            DoFlag = True
            mnuOption.Enabled = False
            If mnuCtlMoveDemo.Checked = True Then
                ' Rend visible le graphisme bondissant (contr�le PictureBox).
                picBall.Visible = True
                ' D�termine le mouvement initial du graphisme bondissant au hasard.
                ' Param�tres compris entre 1 et 4.
                ' La valeur de la variable Motion d�termine
                ' quelle partie de la routine Do Loop s'ex�cute.
                Motion = Int(4 * Rnd + 1)
            ElseIf mnuLineDemo.Checked = True Then
                ' Initialise le g�n�rateur de nombres al�atoires.
                Randomize
                ' D�finit la largeur du trait.
                DrawWidth = 2
                ' D�finit les coordonn�es X et Y initiales en un
                ' point de la feuille pris au hasard.
                X1 = Int(DemoForm.Width * Rnd + 1)
                Y1 = Int(DemoForm.Height * Rnd + 1)
            ElseIf mnuShapeDemo.Checked = True Then
                ' Charge dynamiquement un groupe de contr�les de 20
                ' contr�les Shape sur la feuille.
                For MakeClone = 1 To 20
                    Load shpClone(MakeClone)
                Next MakeClone
            ElseIf mnuPSetDemo.Checked = True Then
                ' D�finit l'�paisseur des confettis.
                DrawWidth = 5
            ElseIf mnuLineCtlDemo.Checked = True Then
                ' Rend visible le contr�le Line.
                linLineCtl.Visible = True
                ' D�finit l'�paisseur du trait tel qu'il appara�tra.
                DrawWidth = 7
            ElseIf mnuImageDemo.Checked = True Then
                ' Rend visible le graphisme bondissant (contr�le Image).
                imgMoon(0).Visible = True
                ' D�finit le cadre d'animation initial.
                FrameNum = 0
                ' D�finit de mani�re al�atoire le d�placement initial du graphisme rebondissant.
                ' Param�tres compris entre 1 et 4.
                ' La valeur de la variable Motion d�termine quelle partie de la routine Do Loop s'ex�cute.
                Motion = Int(4 * Rnd + 1)
            ElseIf mnuScaleDemo.Checked = True Then
                ' Initialise le g�n�rateur de nombres al�atoires.
                Randomize
                ' D�finit la largeur des contours de zones pour que celles-ci ne se recouvrent pas.
                DrawWidth = 1
                ' D�finit la valeur de l'abscisse X du bord gauche de la feuille.
                ' D�finit l'abscisse X � 1 pour la premi�re zone, 2 pour la seconde zone et ainsi de suite.
                ScaleLeft = 1
                ' D�finit l'ordonn�e Y du bord sup�rieur de la feuille � 10.
                ScaleTop = 10
                ' D�finit le nombre d'unit�s comprises dans la largeur de la feuille
                ' avec un nombre al�atoire compris entre 3 et 12. Cela modifie le nombre
                ' de zones dessin�es chaque fois que la routine d�marre.
                ScaleWidth = Int(13 * Rnd + 3)
                ' D�finit le nombre d'unit�s comprises dans la hauteur de la feuille � -10.
                ' Ainsi, la hauteur des zones varie de 0 � 10 et l'ordonn�e Y d�marre au bas
                ' de la feuille.
                ScaleHeight = -10
            ElseIf mnuCircleDemo.Checked = True Then
                ' D�finit la largeur du contour du cercle.
                DrawWidth = 1
                ' Dessine les cercles en traits discontinus.
                DrawStyle = vbDash
                ' Dessine les traits � l'aide du crayon XOR, en combinant les couleurs
                ' du crayon ou de l'affichage, mais pas les deux.
                DrawMode = vbXorPen
            End If
    End Select
End Sub

Private Sub CtlMoveDemo()
    Select Case Motion
    Case 1
        ' D�place le graphisme vers la gauche et vers le haut de 20 twips en utilisant la m�thode Move.
        picBall.Move picBall.Left - 20, picBall.Top - 20
        ' Si le graphisme atteint le bord gauche de la feuille, le d�place vers la droite et vers le haut.
        If picBall.Left <= 0 Then
            Motion = 2
        ' Si le graphisme atteint le bord sup�rieur de la feuille, le d�place vers la gauche et vers le bas.
        ElseIf picBall.Top <= 0 Then
            Motion = 4
        End If
    Case 2
        ' D�place le graphisme vers la droite et vers le haut de 20 twips.
        picBall.Move picBall.Left + 20, picBall.Top - 20

        ' Si le graphisme atteint le bord droit de la feuille, le
        ' d�place vers la gauche et vers le haut. La routine d�termine le
        ' bord droit de la feuille en soustrayant la largeur du
        ' graphisme de la largeur de la feuille.
        If picBall.Left >= (DemoForm.Width - picBall.Width) Then
            Motion = 1
        ' Si le graphisme atteint le bord sup�rieur de la feuille, le d�place vers la droite et vers le bas.
        ElseIf picBall.Top <= 0 Then
            Motion = 3
        End If
    Case 3
        ' D�place le graphisme vers la droite et vers le bas de 20 twips.
        picBall.Move picBall.Left + 20, picBall.Top + 20
        ' Si le graphisme atteint le bord droit de la feuille, le d�place vers la gauche et vers le bas.
        If picBall.Left >= (DemoForm.Width - picBall.Width) Then
            Motion = 4
        ' Si le graphisme atteint le bord inf�rieur de la feuille, le d�place vers la droite et vers le haut.
        ' La routine d�termine le bord inf�rieur de la feuille en soustrayant la hauteur du graphisme
        ' de la hauteur de la feuille moins 680 twips pour tenir compte de la hauteur
        ' de la barre de titre et de la barre de menu.

        ElseIf picBall.Top >= (DemoForm.Height - picBall.Height) - 680 Then
            Motion = 2
        End If
    Case 4
        ' D�place le graphisme vers la gauche et vers le bas de 20 twips.
        picBall.Move picBall.Left - 20, picBall.Top + 20
        ' Si le graphisme atteint le bord gauche de la feuille, le d�place vers la droite et vers le bas.
        If picBall.Left <= 0 Then
            Motion = 3
        ' Si le graphisme atteint le bord inf�rieur de la feuille, le d�place vers la gauche et vers le haut.
        ElseIf picBall.Top >= (DemoForm.Height - picBall.Height) - 680 Then
            Motion = 1
        End If
    End Select
End Sub

Private Sub Delay()
    Dim Start
    Dim Check
    Start = Timer
    Do Until Check >= Start + 0.15
        Check = Timer
    Loop
End Sub

Private Sub Form_Load()
    DoFlag = False
End Sub

Private Sub Form_Resize()
    If mnuScaleDemo.Checked = True And DemoForm.WindowState = 0 Then
        ' Initialise le g�n�rateur de nombres al�atoires.
        Randomize
        ' D�finit une largeur �troite de contours de zone pour que les zones ne se superposent pas.
        DrawWidth = 1
        ' D�finit la valeur de l'abscisse X du bord gauche de la feuille � 1.
        ' Cela permet de d�finir ais�ment la position de chaque zone.
        ' La premi�re poss�de une abscisse �gale � 1, la seconde une abscisse �gale � 2, etc.
        ScaleLeft = 1
        ' D�finit la valeur de l'ordonn�e Y du bord sup�rieur de la feuille � 10.
        ScaleTop = 10
        ' D�finit le nombre d'unit�s dans la largeur de la feuille avec un nombre al�atoire
        ' compris entre 3 et 12. De ce fait, le nombre de zones dessin�es varie chaque fois
        ' que l'utilisateur red�marre la routine.
        ScaleWidth = Int(13 * Rnd + 3)
        ' D�finit le nombre d'unit�s dans la hauteur de la feuille � -10. L'effet est double.
        ' Premi�rement, toutes les zones ont une hauteur qui varie entre 0 et 10.
        ' Deuxi�mement, � cause de la valeur n�gative, les ordonn�es Y commencent � partir
        ' du bord inf�rieur de la feuille au lieu du bord sup�rieur.

        ScaleHeight = -10
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    End
End Sub

Private Sub ImageDemo()
    Select Case Motion
    Case 1
        ' D�place le graphisme vers la gauche et vers le haut de 100 twips � l'aide de la m�thode Move.
        imgMoon(0).Move imgMoon(0).Left - 100, imgMoon(0).Top - 100
        ' Incr�mente l'animation au cadre suivant.
        IncrFrame
        ' Si le graphisme atteint le bord gauche de la feuille, le d�place vers la droite et vers le haut.
        If imgMoon(0).Left <= 0 Then
            Motion = 2
        ' Si le graphisme atteint le bord sup�rieur de la feuille, le d�place vers la gauche et vers le bas.
        ElseIf imgMoon(0).Top <= 0 Then
            Motion = 4
        End If
    Case 2
        ' D�place le graphisme vers la droite et vers le haut de 100 twips.
        imgMoon(0).Move imgMoon(0).Left + 100, imgMoon(0).Top - 100
        ' Incr�mente l'animation au cadre suivant.
        IncrFrame
        ' Si le graphisme atteint le bord droit de la feuille, le d�place vers la gauche et vers le haut.
        ' La routine d�termine le bord droit de la feuille en soustrayant la largeur
        ' du graphisme de celle du contr�le.

        If imgMoon(0).Left >= (DemoForm.Width - imgMoon(0).Width) Then
            Motion = 1
        ' Si le graphisme atteint le bord sup�rieur de la feuille, le d�place vers la droite et vers le bas.
        ElseIf imgMoon(0).Top <= 0 Then
            Motion = 3
        End If
    Case 3
        ' D�place le graphisme vers la droite et vers le bas de 100 twips.
        imgMoon(0).Move imgMoon(0).Left + 100, imgMoon(0).Top + 100
        ' Incr�mente l'animation au cadre suivant.
        IncrFrame
        ' Si le graphisme atteint le bord droit de la feuille, le d�place vers la gauche et vers le bas.
        If imgMoon(0).Left >= (DemoForm.Width - imgMoon(0).Width) Then
            Motion = 4
        ' Si le graphisme atteint le bord inf�rieur de la feuille, le d�place vers la droite et vers le haut.
        ' La routine d�termine le bord inf�rieur de la feuille en soustrayant la hauteur du
        ' graphisme de la hauteur de la feuille moins 680 twips pour tenir compte de la hauteur
        ' de la barre de titre et de la barre de menu.
        ElseIf imgMoon(0).Top >= (DemoForm.Height - imgMoon(0).Height) - 680 Then
            Motion = 2
        End If
    Case 4
        ' D�place le graphisme vers la gauche et vers le bas de 100 twips.
        imgMoon(0).Move imgMoon(0).Left - 100, imgMoon(0).Top + 100
        ' Incr�mente l'animation au cadre suivant.
        IncrFrame
        ' Si le graphisme atteint le bord gauche de la feuille, le d�place vers la droite et vers le bas.
        If imgMoon(0).Left <= 0 Then
            Motion = 3
        ' Si le graphisme atteint le bord inf�rieur de la feuille, le d�place vers la gauche et vers le haut.
        ElseIf imgMoon(0).Top >= (DemoForm.Height - imgMoon(0).Height) - 680 Then
            Motion = 1
        End If
    End Select
End Sub

Private Sub IncrFrame()
    ' Incr�mente le num�ro de cadre.
    FrameNum = FrameNum + 1
    ' Le groupe de contr�les avec cadres d'animation comporte les �l�ments 0 � 7.
    ' Au 8e cadre, r�initialise le num�ro de cadrer � 1 pour cr�er une boucle d'animation sans fin.
    If FrameNum > 8 Then
        FrameNum = 1
    End If
    ' D�finit pour la propri�t� du contr�le Image la propri�t� Image du cadre en cours.
    imgMoon(0).Picture = imgMoon(FrameNum).Picture
    ' Marque une pause dans l'affichage pour ralentir l'animation.
    Me.Refresh
    Delay
End Sub

Private Sub LineCtlDemo()
    ' D�finit les coordonn�es X et Y du d�but de ligne (position gauche/droite)
    ' en un point de la feuille pris au hasard.
    linLineCtl.X1 = Int(DemoForm.Width * Rnd)
    linLineCtl.Y1 = Int(DemoForm.Height * Rnd)
    ' D�finit les coordonn�es X et Y de la fin de ligne (position gauche/droite)
    ' en un point de la feuille pris au hasard.
    linLineCtl.X2 = Int(DemoForm.Width * Rnd)
    linLineCtl.Y2 = Int(DemoForm.Height * Rnd)
    ' Efface la feuille pour supprimer les pixels restants.
    Cls
    ' Marque une pause dans l'affichage avant de red�placer le trait.
    Delay
End Sub

Private Sub LineDemo()
    ' D�clare des variables locales.
    Dim X2
    Dim Y2
    ' Cr�e des couleurs RGB au hasard.
    R = 255 * Rnd
    G = 255 * Rnd
    B = 255 * Rnd
    ' D�finit l'extr�mit� finale du trait en un point pris au hasard dans la feuille.
    X2 = Int(DemoForm.Width * Rnd + 1)
    Y2 = Int(DemoForm.Height * Rnd + 1)
    ' A l'aide de la M�thode Line, dessine depuis les coordonn�es en cours jusqu'� l'extr�mit�
    ' en cours, en donnant une couleur al�atoire au trait. Chaque trait commence au point
    ' o� le pr�c�dent se termine.
    Line -(X2, Y2), RGB(R, G, B)
End Sub

Private Sub mnuCircleDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = True
End Sub

Private Sub mnuCtlMoveDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = True
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuExit_Click()
    End
End Sub

Private Sub mnuImageDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = True
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuLineCtlDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = True
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuLineDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = True
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuPSetDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = True
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuScaleDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = False
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = True
    mnuCircleDemo.Checked = False
End Sub

Private Sub mnuShapeDemo_Click()
    Cls
    mnuCtlMoveDemo.Checked = False
    mnuLineDemo.Checked = False
    mnuShapeDemo.Checked = True
    mnuPSetDemo.Checked = False
    mnuLineCtlDemo.Checked = False
    mnuImageDemo.Checked = False
    mnuScaleDemo.Checked = False
    mnuCircleDemo.Checked = False
End Sub

Private Sub PSetDemo()
    ' Cr�e des couleurs RGB al�atoires.
    R = 255 * Rnd
    G = 255 * Rnd
    B = 255 * Rnd
    ' XPos d�finit la position horizontale d'un confetti en un point pris au hasard dans la feuille.
    XPos = Rnd * ScaleWidth
    ' YPos d�finit la position verticale d'un confetti en un point pris au hasard dans la feuille.
    YPos = Rnd * ScaleHeight
    ' Dessine un confetti au point XPos, Ypos. Affecte au confetti une couleur al�atoire.
    PSet (XPos, YPos), RGB(R, G, B)
End Sub

Private Sub ScaleDemo()
    ' D�clare des variables locales.
    Dim Box
    ' Cr�e autant de zones que d'unit�s dans la largeur de la feuille.
    For Box = 1 To ScaleWidth
        ' Cr�e des couleurs RGB al�atoires.
        R = 255 * Rnd
        G = 255 * Rnd
        B = 255 * Rnd
        ' Dessine des zones � l'aide de la m�thode Line et des options B (zone), F (remplie).
        ' Les zones commencent � chaque abscisse X d�termin�e par la propri�t� ScaleWidth
        ' et � une ordonn�e Y �gale � 0 (bas de la feuille). Chaque zone mesure 1 unit� de large
        ' et poss�de une hauteur al�atoire comprise entre 0 et 10. Remplit la zone d'une couleur al�atoire.
        Line (Box, 0)-Step(1, (Int(11 * Rnd))), RGB(R, G, B), BF
    Next Box
    ' Pause pour l'affichage de toutes les zones avant de redessiner.
    Delay
End Sub

Private Sub ShapeDemo()
    ' D�clare des variables locales.
    Dim CloneID
    ' Cr�e des couleurs RGB al�atoires.
    R = 255 * Rnd
    G = 255 * Rnd
    B = 255 * Rnd
    ' D�finit une couleur al�atoire pour l'arri�re-plan de la feuille.
    DemoForm.BackColor = RGB(R, G, B)
    ' S�lectionne un contr�le Shape au hasard dans le groupe de contr�les.
    CloneID = Int(20 * Rnd + 1)
    ' XPos et Ypos d�finissent la position du contr�le Shape s�lectionn� en un point
    ' pris au hasard dans la feuille.
    XPos = Int(DemoForm.Width * Rnd + 1)
    YPos = Int(DemoForm.Height * Rnd + 1)
    ' D�finit une figure au hasard pour le contr�le Shape s�lectionn�.
    shpClone(CloneID).Shape = Int(6 * Rnd)
    ' D�finit la hauteur et la largeur du contr�le Shape s�lectionn� � une dimension au hasard
    ' comprise entre 500 et 2500 twips.
    shpClone(CloneID).Height = Int(2501 * Rnd + 500)
    shpClone(CloneID).Width = Int(2501 * Rnd + 500)
    ' D�finit la couleur d'arri�re-plan et DrawMode du contr�le Shape � une couleur al�atoire.
    shpClone(CloneID).BackColor = QBColor(Int(15 * Rnd))
    shpClone(CloneID).DrawMode = Int(16 * Rnd + 1)
    ' D�place le contr�le Figure s�lectionn� � l'aide de la m�thode Move
    shpClone(CloneID).Move XPos, YPos
    ' Rend visible le contr�le Shape s�lectionn�.
    shpClone(CloneID).Visible = True
    ' Attend bri�vement avant s�lection et modification du contr�le Shape suivant.
    Delay
End Sub

Private Sub Timer1_Timer()
    If mnuCtlMoveDemo.Checked And DoFlag = True Then
        CtlMoveDemo
    ElseIf mnuLineDemo.Checked And DoFlag = True Then
        LineDemo
    ElseIf mnuShapeDemo.Checked And DoFlag = True Then
        ShapeDemo
    ElseIf mnuPSetDemo.Checked And DoFlag = True Then
        PSetDemo
    ElseIf mnuLineCtlDemo.Checked And DoFlag = True Then
        LineCtlDemo
    ElseIf mnuImageDemo.Checked And DoFlag = True Then
        ImageDemo
    ElseIf mnuScaleDemo.Checked And DoFlag = True Then
        ScaleDemo
    ElseIf mnuCircleDemo.Checked And DoFlag = True Then
        CircleDemo
    End If
End Sub

