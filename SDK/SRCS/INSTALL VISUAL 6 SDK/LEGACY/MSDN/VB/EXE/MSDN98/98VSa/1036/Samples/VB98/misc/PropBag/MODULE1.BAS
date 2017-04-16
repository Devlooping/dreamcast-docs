Attribute VB_Name = "modDHTML"
'PutProperty: Stocke des informations dans un cookie en appelant cette
'             fonction.
'             Entr�es: Propri�t� nomm�e
'             et valeur de la propri�t� � stocker.
'
'             Entr�es facultatives:
'             expires : date qui d�finit la dur�e de vie valide
'                       de la propri�t�. Une fois la date d'expiration
'                       atteinte, n'est plus stock�e ni restitu�e.

Public Sub PutProperty(objDocument As HTMLDocument, strName As String, vntValue As Variant, Optional Expires As Date)

     objDocument.cookie = strName & "=" & CStr(vntValue) & _
        IIf(CLng(Expires) = 0, "", "; expires=" & Format(CStr(Expires), "ddd, dd-mmm-yy hh:mm:ss") & " GMT") ' & _

End Sub

'GetProperty: Extrait la valeur d'une propri�t� en appelant cette
'             fonction. L'entr�e est la propri�t� nomm�e
'             et la valeur de retour de la fonction est la valeur actuelle
'             de la propri�t�. Si la propri�t� est absente ou p�rim�e,
'             la valeur de retour est une cha�ne vide.
'
Public Function GetProperty(objDocument As HTMLDocument, strName As String) As Variant
    Dim aryCookies() As String
    Dim strCookie As Variant
    On Local Error GoTo NextCookie

    ' Fractionne l'objet cookie du document en tableau de cookies.
    aryCookies = Split(objDocument.cookie, ";")
    For Each strCookie In aryCookies
        If Trim(VBA.Left(strCookie, InStr(strCookie, "=") - 1)) = Trim(strName) Then
            GetProperty = Trim(Mid(strCookie, InStr(strCookie, "=") + 1))
            Exit Function
        End If
NextCookie:
        Err = 0
    Next strCookie
End Function


