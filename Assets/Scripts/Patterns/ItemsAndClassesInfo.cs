using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Custom/ItemsAndClassesInfo")]
public class ItemsAndClassesInfo : ScriptableObject
{
    public ItemPattern[] items;

    public ClassInfo[] classes;
    public ClassInfo[] enemyPatterns;

    public EffectInfo[] effects;

    public Skill GetSkillByID(int id)
    {
        foreach (ClassInfo info in classes)
        {
            foreach(Skill skill in info.skills)
            {
                if (skill.id == id)
                    return skill;
            }
        }
        foreach (ClassInfo info in enemyPatterns)
        {
            foreach (Skill skill in info.skills)
            {
                if (skill.id == id)
                    return skill;
            }
        }
        return null;

    }

    public List<ArmorPattern> GetArmors()
    {
        List<ArmorPattern> res = new List<ArmorPattern>();
        foreach (ItemPattern item in items)
        {
            if (item is ArmorPattern)
                res.Add((ArmorPattern)item);
        }
        return res;
    }
    public List<WeaponPattern> GetWeapons()
    {
        List<WeaponPattern> res = new List<WeaponPattern>();
        foreach (ItemPattern item in items)
        {
            if (item is WeaponPattern)
                res.Add((WeaponPattern)item);
        }
        return res;
    }
    public int GetPatternID(ItemPattern pattern)
    {
       for(int i = 0; i < items.Length; i++)
       {
            if(items[i] == pattern)
                return i;
       }
       return 0;
    }

    public ClassInfo GetClassPatternByType(Class type)
    {
        foreach (ClassInfo info in classes)
        {
            if (info.mercenaryClass == type)
                return info;
        }
        foreach (ClassInfo info in enemyPatterns)
        {
            if (info.mercenaryClass == type)
                return info;
        }
        return null;

    }

    public WeaponPattern GetWeaponPatternByEnam(WeaponType type)
    {
        foreach(ItemPattern item in items)
        {
            if(item is WeaponPattern && ((WeaponPattern)item).type == type)
                return ((WeaponPattern)item);
        }
        return null;
    }

    public ArmorPattern GetArmorPatternByEnam(ArmorType type)
    {
        foreach (ItemPattern item in items)
        {
            if (item is ArmorPattern && ((ArmorPattern)item).type == type)
                return ((ArmorPattern)item);
        }
        return null;
    }
}

