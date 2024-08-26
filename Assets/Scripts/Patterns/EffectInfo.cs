using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[CreateAssetMenu(menuName = "Custom/EffectInfo")]
public class EffectInfo : ScriptableObject
{
    public string title;
    [TextArea(3, 6)]
    public string description;

    public List<EffectAction> actions;

}

[System.Serializable]
public class EffectAction
{
    public EffectActopnType type;
    public int effectPower;
}
public enum EffectActopnType
{
    ChangeMaxHp,
    ChangeDamage,
    ChangeProtection,
    ChangeEvasionChance,
    ChangeCriticalStrikeChance,
    DealingDamageEveryMove,
    DealingDamageEveryDay,
}
