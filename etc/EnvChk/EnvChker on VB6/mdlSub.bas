Attribute VB_Name = "mdlSub"
Option Explicit

Public Function Max(Value1, Value2)
    Max = IIf(Value1 > Value2, Value1, Value2)
End Function

Public Function GetNamedChildNode(BaseNode As IXMLDOMNode, Name As String) As IXMLDOMNode
    Dim i As Long
    Dim node As IXMLDOMNode
    
    For i = 0 To BaseNode.childNodes.Length - 1
        If UCase(BaseNode.childNodes(i).nodeName) = UCase(Name) Then
            Set GetNamedChildNode = BaseNode.childNodes(i)
            Exit Function
        End If
    Next

    Set GetNamedChildNode = Nothing
End Function
