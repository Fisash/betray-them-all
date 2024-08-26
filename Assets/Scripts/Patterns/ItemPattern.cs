using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Custom/BaseItemPattern")]
public class ItemPattern : ScriptableObject
{
    public string title;

    public string baseDescription;

    public bool isItUsableInFight;

    public Sprite icon;
    public int cost;

    public virtual string GetDescription(Item item)
    {
        return baseDescription;
    }
}
