using Godot;
using Epic.OnlineServices;

public class ContinuanceTokenWrapper : Reference
{
    public ContinuanceToken _internalToken;

    public ContinuanceTokenWrapper(ContinuanceToken token)
    {
        _internalToken = token;
    }

    public string GetClass()
    {
        return "ContinuanceToken";
    }

    public override string ToString()
    {
        return "ContinuanceTokenWrapper(" + _internalToken.ToString() + ")";
    }
}