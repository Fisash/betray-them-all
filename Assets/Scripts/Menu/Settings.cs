using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

public class Settings : MonoBehaviour
{
    public SettingsPanel panel;
    public string savePath;

    string fullPath;
    public void Start()
    {
        if (!PlayerPrefs.HasKey("savePath"))
        {
            PlayerPrefs.SetString("savePath", savePath);
        }
        savePath = PlayerPrefs.GetString("savePath");
        fullPath = savePath + @"\settings.settings";
        LoadSettings();
    }

    public void LoadSettings()
    {
        if (File.Exists(fullPath))
        {
            using (FileStream fileStream = new FileStream(fullPath, FileMode.Open))
            {
                    BinaryFormatter formatter = new BinaryFormatter();
                    SettingsData data = (SettingsData)formatter.Deserialize(fileStream);
                    UpdateVolume(data);
                    panel.Load(data);
                    
            }
        }
        else
        {
            SettingsData data = new SettingsData();
            SaveSettings(data);
            LoadSettings();
        }
    }
    public void SaveSettings(SettingsData data)
    {
        
        string directoryPath = Path.GetDirectoryName(fullPath);

        if (!Directory.Exists(directoryPath))
            Directory.CreateDirectory(directoryPath);

        using (FileStream fileStream = new FileStream(fullPath, FileMode.OpenOrCreate))
        {
            BinaryFormatter formatter = new BinaryFormatter();
            formatter.Serialize(fileStream, data);
            UpdateVolume(data);
        }
    }

    public void UpdateVolume(SettingsData data)
    {
        GameObject.Find("SoundManager").GetComponent<LerpSound>().LerpTo(data.soundVolume / 100f);
        GameObject.Find("Main Camera").GetComponent<LerpSound>().LerpTo(data.musicVolume / 100f);
    }
}


[System.Serializable]
public class SettingsData
{
    public bool isAvoidgMountains;
    public bool isAvoidgForests;

    public int soundVolume;
    public int musicVolume;

    public SettingsData(bool isAvoidgMountains = true, bool isAvoidgForests = false, int soundVolume = 80, int musicVolume = 70)
    {
        this.isAvoidgMountains = isAvoidgMountains;
        this.isAvoidgForests = isAvoidgForests;
        this.soundVolume = soundVolume;
        this.musicVolume = musicVolume;
    }
}